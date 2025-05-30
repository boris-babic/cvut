#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <optional>
#include <memory>
#include <stdexcept>
#include <set>
#include <map>
#include <queue>
#include <deque>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#endif /* __PROGTEST__ */

class CLinker
{
  public:
    CLinker & addFile ( const std::string & fileName )
    {
      loadObjectFile ( fileName );
      return *this;
    }

    void linkOutput ( const std::string & fileName, const std::string & entryPoint )
    {
      performLinking ( fileName, entryPoint );
    }

  private:
    struct ExportEntry {
      std::string_view name;
      uint32_t         offset;
      uint32_t         length;
    };

    struct ImportEntry {
      std::string_view     name;
      std::vector<uint32_t> refOffsets;
    };

    struct ObjectFile {
      std::vector<uint8_t>  codeBlock;
      std::vector<ExportEntry> exports;
      std::vector<ImportEntry> imports;
      uint32_t              codeSize;
    };

    struct GlobalSymbol {
      uint32_t objIndex : 16;
      uint32_t exportIndex : 15;
      uint32_t placed : 1;
      uint32_t outAddr;
    };

    std::vector<ObjectFile> objectFiles;
    std::unordered_map<std::string_view, GlobalSymbol> symbolTable;
    std::vector<std::string_view> linkedOrder;

    // String interning
    std::unordered_map<std::string, std::string> internPool;
    const std::string_view intern ( const std::string & s )
    {
      auto [it, _] = internPool.emplace(s, s);
      return std::string_view(it->first);
    }

    void loadObjectFile ( const std::string & fileName )
    {
      ObjectFile obj;
      std::ifstream in ( fileName.c_str(), std::ios::binary );
      if ( ! in ) throw std::runtime_error ( "Nepodarilo sa otvoriť súbor: " + fileName );

      uint32_t numExports, numImports, codeSize;
      in.read ( (char *) &numExports, sizeof(uint32_t) );
      in.read ( (char *) &numImports, sizeof(uint32_t) );
      in.read ( (char *) &codeSize, sizeof(uint32_t) );
      obj.codeSize = codeSize;

      for ( uint32_t i = 0; i < numExports; i++ )
      {
        ExportEntry exp;
        uint8_t nameLen;
        char buffer[256];
        in.read ( (char *) &nameLen, sizeof(uint8_t) );
        if ( nameLen > 0 ) in.read ( buffer, nameLen );
        std::string temp ( buffer, nameLen );
        exp.name = intern(temp);
        in.read ( (char *) &exp.offset, sizeof(uint32_t) );
        obj.exports.push_back ( exp );
      }

      for ( uint32_t i = 0; i < numImports; i++ )
      {
        ImportEntry imp;
        uint8_t nameLen;
        char buffer[256];
        in.read ( (char *) &nameLen, sizeof(uint8_t) );
        if ( nameLen > 0 ) in.read ( buffer, nameLen );
        std::string temp ( buffer, nameLen );
        imp.name = intern(temp);
        uint32_t numRefs;
        in.read ( (char *) &numRefs, sizeof(uint32_t) );
        imp.refOffsets.reserve(numRefs);
        for ( uint32_t j = 0; j < numRefs; j++ )
        {
          uint32_t ref;
          in.read ( (char *) &ref, sizeof(uint32_t) );
          imp.refOffsets.push_back ( ref );
        }
        obj.imports.push_back ( imp );
      }

      obj.codeBlock.resize ( codeSize );
      in.read ( (char *) obj.codeBlock.data(), codeSize );
      if ( ! in ) throw std::runtime_error ( "Chyba pri čítaní súboru: " + fileName );

      std::vector<ExportEntry> sorted = obj.exports;
      std::sort ( sorted.begin(), sorted.end(),
                  [] ( const ExportEntry & a, const ExportEntry & b ) {
                    return a.offset < b.offset;
                  } );
      for ( size_t i = 0; i < sorted.size(); i++ )
      {
        uint32_t start = sorted[i].offset;
        uint32_t end = ( i + 1 < sorted.size() ) ? sorted[i+1].offset : obj.codeSize;
        uint32_t len = end - start;
        for ( auto & e : obj.exports )
          if ( e.name == sorted[i].name && e.offset == start ) {
            e.length = len;
            break;
          }
      }

      for ( size_t i = 0; i < obj.exports.size(); i++ )
      {
        const ExportEntry & exp = obj.exports[i];
        if ( symbolTable.find ( exp.name ) != symbolTable.end() )
          throw std::runtime_error ( "Duplicita symbolu: " + std::string(exp.name) );
        GlobalSymbol gs;
        gs.objIndex = objectFiles.size();
        gs.exportIndex = i;
        gs.outAddr = 0;
        gs.placed = false;
        symbolTable[ exp.name ] = gs;
      }

      obj.codeBlock.shrink_to_fit();
      obj.exports.shrink_to_fit();
      obj.imports.shrink_to_fit();
      objectFiles.push_back ( std::move(obj) );
    }

    void resolveDependencies ( const std::string & symbol )
    {
      auto it = symbolTable.find ( symbol );
      if ( it == symbolTable.end() )
        throw std::runtime_error ( "Symbol " + symbol + " nebol nájdený." );
      if ( it->second.placed ) return;
      it->second.placed = true;
      linkedOrder.push_back ( it->first );

      const ObjectFile & obj = objectFiles[it->second.objIndex];
      const ExportEntry & exp = obj.exports[it->second.exportIndex];
      uint32_t start = exp.offset;
      uint32_t end = start + exp.length;

      for ( const auto & imp : obj.imports )
        for ( uint32_t ref : imp.refOffsets )
          if ( ref >= start && ref < end )
            resolveDependencies ( std::string(imp.name) );
    }

    void performLinking ( const std::string & outFileName, const std::string & entryPoint )
    {
      if ( symbolTable.find ( entryPoint ) == symbolTable.end() )
        throw std::runtime_error ( "Entry point symbol " + entryPoint + " neexistuje." );

      linkedOrder.clear();
      linkedOrder.reserve(1024); // reasonable default
      resolveDependencies ( entryPoint );

      std::vector<uint8_t> output;
      std::map<std::string_view, uint32_t> symOut;

      for ( const auto & sym : linkedOrder )
      {
        GlobalSymbol & gs = symbolTable[sym];
        const ObjectFile & obj = objectFiles[ gs.objIndex ];
        const ExportEntry & exp = obj.exports[ gs.exportIndex ];
        gs.outAddr = output.size();
        symOut[sym] = gs.outAddr;

        if ( exp.offset + exp.length > obj.codeSize )
          throw std::runtime_error ( "Chybné exportovanie pre symbol " + std::string(sym) );

        output.insert ( output.end(),
                        obj.codeBlock.begin() + exp.offset,
                        obj.codeBlock.begin() + exp.offset + exp.length );
      }

      for ( const auto & sym : linkedOrder )
      {
        GlobalSymbol & gs = symbolTable[sym];
        const ObjectFile & obj = objectFiles[ gs.objIndex ];
        const ExportEntry & exp = obj.exports[ gs.exportIndex ];
        uint32_t start = exp.offset;
        uint32_t end = exp.offset + exp.length;
        for ( const auto & imp : obj.imports )
          for ( uint32_t ref : imp.refOffsets )
            if ( ref >= start && ref < end )
            {
              size_t pos = gs.outAddr + (ref - start);
              if ( pos + sizeof(uint32_t) > output.size() )
                throw std::runtime_error ( "Nesprávny offset pri patchovaní symbolu " + std::string(sym) );
              if ( symOut.find ( imp.name ) == symOut.end() )
                throw std::runtime_error ( "Nezaradený import: " + std::string(imp.name) );
              uint32_t addr = symOut[ imp.name ];
              std::memcpy ( &output[pos], &addr, sizeof(uint32_t) );
            }
      }

      std::ofstream out ( outFileName.c_str(), std::ios::binary );
      if ( ! out ) throw std::runtime_error ( "Nepodarilo sa otvoriť výstupný súbor: " + outFileName );
      out.write ( (char *) output.data(), output.size() );
      if ( ! out ) throw std::runtime_error ( "Chyba pri zápise do súboru: " + outFileName );
    }
};
#ifndef __PROGTEST__
int main ()
{
  CLinker () . addFile ( "0in0.o" ) . linkOutput ( "0out", "strlen" );

  CLinker () . addFile ( "1in0.o" ) . linkOutput ( "1out", "main" );

  CLinker () . addFile ( "2in0.o" ) . addFile ( "2in1.o" ) . linkOutput ( "2out", "main" );

  CLinker () . addFile ( "3in0.o" ) . addFile ( "3in1.o" ) . linkOutput ( "3out", "towersOfHanoi" );

  try
  {
    CLinker () . addFile ( "4in0.o" ) . addFile ( "4in1.o" ) . linkOutput ( "4out", "unusedFunc" );
    assert ( "missing an exception" == nullptr );
  }
  catch ( const std::runtime_error & e )
  {
    // e . what (): Undefined symbol qsort
  }
  catch ( ... )
  {
    assert ( "invalid exception" == nullptr );
  }

  try
  {
    CLinker () . addFile ( "5in0.o" ) . linkOutput ( "5out", "main" );
    assert ( "missing an exception" == nullptr );
  }
  catch ( const std::runtime_error & e )
  {
    // e . what (): Duplicate symbol: printf
  }
  catch ( ... )
  {
    assert ( "invalid exception" == nullptr );
  }

  try
  {
    CLinker () . addFile ( "6in0.o" ) . linkOutput ( "6out", "strlen" );
    assert ( "missing an exception" == nullptr );
  }
  catch ( const std::runtime_error & e )
  {
    // e . what (): Cannot read input file
  }
  catch ( ... )
  {
    assert ( "invalid exception" == nullptr );
  }

  try
  {
    CLinker () . addFile ( "7in0.o" ) . linkOutput ( "7out", "strlen2" );
    assert ( "missing an exception" == nullptr );
  }
  catch ( const std::runtime_error & e )
  {
    // e . what (): Undefined symbol strlen2
  }
  catch ( ... )
  {
    assert ( "invalid exception" == nullptr );
  }

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
