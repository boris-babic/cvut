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
    // Verejné metódy - rozhranie sa nemení
    CLinker & addFile(const std::string & fileName)
    {
        loadObjectFile(fileName);
        return *this;
    }
    void linkOutput(const std::string & fileName,
                    const std::string & entryPoint)
    {
        performLinking(fileName, entryPoint);
    }
    
  private:
    // Štruktúra pre exportovaný symbol
    struct ExportEntry {
        std::string name;   // názov funkcie
        uint32_t offset;    // offset od začiatku kódu
        uint32_t length;    // vypočítaná dĺžka funkcie
    };

    // Štruktúra pre importovaný symbol
    struct ImportEntry {
        std::string name;               // názov importovanej funkcie
        std::vector<uint32_t> refOffsets; // zoznam offsetov, kde sa má zapísať adresa
    };

    // Štruktúra reprezentujúca načítaný objektový súbor
    struct ObjectFile {
        std::string fileName;             // pre diagnostiku
        std::vector<uint8_t> codeBlock;   // blok bajtov so strojovým kódom
        std::vector<ExportEntry> exports;
        std::vector<ImportEntry> imports;
        uint32_t codeSize = 0;            // celková veľkosť kódu (v bajtoch)
    };

    // Uložené načítané objektové súbory
    std::vector<ObjectFile> objectFiles;

    // Globálna tabuľka symbolov, kde každý symbol je definovaný raz.
    struct GlobalSymbol {
        int objIndex;       // index objektového súboru v poli objectFiles
        int exportIndex;    // index záznamu v poli exports daného objektu
        uint32_t outAddr = 0; // absolútna adresa, kde bude funkcia umiestnená vo výslednom súbore
        bool placed = false;  // príznak, či už bola funkcia pridaná do výsledného obrazu
    };
    std::unordered_map<std::string, GlobalSymbol> symbolTable;

    // Poradie, v akom budú funkcie vložené do výsledného výstupu.
    // Prvý musí byť entry point.
    std::vector<std::string> linkedOrder;

    // Pomocná funkcia: načíta jeden objektový súbor podľa zadaného formátu.
    void loadObjectFile(const std::string & fileName)
    {
        ObjectFile obj;
        obj.fileName = fileName;
        std::ifstream in(fileName, std::ios::binary);
        if (!in)
        {
            throw std::runtime_error("Nepodarilo sa otvoriť súbor: " + fileName);
        }
        // Načítanie hlavičky: počet exportov, počet importov, veľkosť kódu.
        uint32_t numExports, numImports, codeSize;
        in.read(reinterpret_cast<char*>(&numExports), sizeof(uint32_t));
        in.read(reinterpret_cast<char*>(&numImports), sizeof(uint32_t));
        in.read(reinterpret_cast<char*>(&codeSize), sizeof(uint32_t));
        obj.codeSize = codeSize;

        // Načítanie tabuľky exportov.
        for (uint32_t i = 0; i < numExports; i++)
        {
            ExportEntry exp;
            uint8_t nameLen;
            in.read(reinterpret_cast<char*>(&nameLen), sizeof(uint8_t));
            char buffer[256];
            if (nameLen > 0)
            {
                in.read(buffer, nameLen);
            }
            exp.name = std::string(buffer, nameLen);
            in.read(reinterpret_cast<char*>(&exp.offset), sizeof(uint32_t));
            obj.exports.push_back(exp);
        }
        // Načítanie tabuľky importov.
        for (uint32_t i = 0; i < numImports; i++)
        {
            ImportEntry imp;
            uint8_t nameLen;
            in.read(reinterpret_cast<char*>(&nameLen), sizeof(uint8_t));
            char buffer[256];
            if (nameLen > 0)
            {
                in.read(buffer, nameLen);
            }
            imp.name = std::string(buffer, nameLen);
            uint32_t numRefs;
            in.read(reinterpret_cast<char*>(&numRefs), sizeof(uint32_t));
            for (uint32_t r = 0; r < numRefs; r++)
            {
                uint32_t ref;
                in.read(reinterpret_cast<char*>(&ref), sizeof(uint32_t));
                imp.refOffsets.push_back(ref);
            }
            obj.imports.push_back(imp);
        }
        // Načítanie bloku strojového kódu.
        obj.codeBlock.resize(codeSize);
        in.read(reinterpret_cast<char*>(obj.codeBlock.data()), codeSize);
        if (!in)
        {
            throw std::runtime_error("Chyba pri čítaní súboru: " + fileName);
        }

        // Výpočet dĺžky funkcií: zoradíme exportované funkcie podľa offsetu.
        std::vector<ExportEntry> sortedExports = obj.exports;
        std::sort(sortedExports.begin(), sortedExports.end(),
                  [](const ExportEntry &a, const ExportEntry &b)
                  {
                      return a.offset < b.offset;
                  });
        for (size_t i = 0; i < sortedExports.size(); i++)
        {
            uint32_t start = sortedExports[i].offset;
            uint32_t end = (i + 1 < sortedExports.size()) ? sortedExports[i+1].offset : obj.codeSize;
            uint32_t len = end - start;
            // Aktualizujeme pôvodný záznam
            for (auto &e : obj.exports)
            {
                if (e.name == sortedExports[i].name && e.offset == start)
                {
                    e.length = len;
                    break;
                }
            }
        }

        // Aktualizácia globálnej tabuľky symbolov, kontrola duplicít.
        for (size_t i = 0; i < obj.exports.size(); i++)
        {
            const auto &exp = obj.exports[i];
            if (symbolTable.find(exp.name) != symbolTable.end())
            {
                throw std::runtime_error("Duplicita symbolu: " + exp.name);
            }
            GlobalSymbol gs;
            gs.objIndex = objectFiles.size();
            gs.exportIndex = i;
            gs.outAddr = 0;
            gs.placed = false;
            symbolTable[exp.name] = gs;
        }

        objectFiles.push_back(std::move(obj));
    }

    // Pomocná funkcia: rekurzívne vyrieši závislosti – prida funkciu do výsledného poradia, ak ešte nebola pridaná.
    void resolveDependencies(const std::string & symbol)
    {
        auto it = symbolTable.find(symbol);
        if (it == symbolTable.end())
        {
            throw std::runtime_error("Symbol " + symbol + " nebol nájdený.");
        }
        if (it->second.placed)
            return;
        it->second.placed = true;
        linkedOrder.push_back(symbol);
        int objIndex = it->second.objIndex;
        int expIndex = it->second.exportIndex;
        ObjectFile &obj = objectFiles[objIndex];
        ExportEntry &exp = obj.exports[expIndex];
        uint32_t funcStart = exp.offset;
        uint32_t funcEnd = exp.offset + exp.length;
        // Pre každý import, ktorý spadá do rozsahu danej funkcie, rekurzívne vyrieš závislosť.
        for (const auto &imp : obj.imports)
        {
            for (uint32_t refOff : imp.refOffsets)
            {
                if (refOff >= funcStart && refOff < funcEnd)
                {
                    resolveDependencies(imp.name);
                }
            }
        }
    }

    // Hlavná implementácia linkovania – vytvori finálny výstup a opraví adresy.
    void performLinking(const std::string & outFileName, const std::string & entryPoint)
    {
        // Kontrola, že entry point existuje.
        if (symbolTable.find(entryPoint) == symbolTable.end())
        {
            throw std::runtime_error("Entry point symbol " + entryPoint + " neexistuje.");
        }
        linkedOrder.clear();
        resolveDependencies(entryPoint);

        // Výstupný obraz ako vektor bajtov.
        std::vector<uint8_t> output;
        // Mapa pre absolútne adresy symbolov vo výslednom výstupe.
        std::unordered_map<std::string, uint32_t> symbolOutAddr;

        // Postupne vkladáme funkcie podľa poradia v linkedOrder.
        for (const auto &sym : linkedOrder)
        {
            auto &gs = symbolTable[sym];
            ObjectFile &obj = objectFiles[gs.objIndex];
            ExportEntry &exp = obj.exports[gs.exportIndex];
            gs.outAddr = static_cast<uint32_t>(output.size());
            symbolOutAddr[sym] = gs.outAddr;
            if(exp.offset + exp.length > obj.codeSize)
            {
                throw std::runtime_error("Chybné exportovanie pre symbol " + sym);
            }
            output.insert(output.end(),
                          obj.codeBlock.begin() + exp.offset,
                          obj.codeBlock.begin() + exp.offset + exp.length);
        }

        // Oprava referencií – prechádzame funkcie a v ich kóde patchujeme importy.
        for (const auto &sym : linkedOrder)
        {
            auto &gs = symbolTable[sym];
            ObjectFile &obj = objectFiles[gs.objIndex];
            ExportEntry &exp = obj.exports[gs.exportIndex];
            uint32_t funcStart = exp.offset;
            uint32_t funcEnd = exp.offset + exp.length;
            for (const auto &imp : obj.imports)
            {
                for (uint32_t refOff : imp.refOffsets)
                {
                    if (refOff >= funcStart && refOff < funcEnd)
                    {
                        uint32_t relativeOff = refOff - funcStart;
                        size_t absPos = gs.outAddr + relativeOff;
                        if (absPos + sizeof(uint32_t) > output.size())
                        {
                            throw std::runtime_error("Chybné offset pre patchovanie symbolu " + sym);
                        }
                        if (symbolOutAddr.find(imp.name) == symbolOutAddr.end())
                        {
                            throw std::runtime_error("Neriadený import: " + imp.name);
                        }
                        uint32_t targetAddr = symbolOutAddr[imp.name];
                        // Zapíšeme 4 bajty adresy v little-endian poradí.
                        std::memcpy(&output[absPos], &targetAddr, sizeof(uint32_t));
                    }
                }
            }
        }

        // Zapísanie výstupného obrazu do súboru.
        std::ofstream out(outFileName, std::ios::binary);
        if (!out)
        {
            throw std::runtime_error("Nepodarilo sa otvoriť výstupný súbor: " + outFileName);
        }
        out.write(reinterpret_cast<const char*>(output.data()), output.size());
        if (!out)
        {
            throw std::runtime_error("Chyba pri zápise do súboru: " + outFileName);
        }
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
