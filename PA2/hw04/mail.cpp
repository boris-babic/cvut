#ifndef __PROGTEST__
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <compare>
#include <functional>
#include <optional>

int compare_ints(const int a, const int b)
{

    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}
class CTimeStamp
{
private:
  int year;
  int month;
  int day;
  int hour;
  int minute;
  double second;

public:
  CTimeStamp() = default;
  CTimeStamp(int year,
             int month,
             int day,
             int hour,
             int minute,
             double sec)
  : year(year), month(month), day(day), hour(hour), minute(minute), second(sec) {}
  int compare(const CTimeStamp &x) const
  {
    if (this->year != x.year)
    {
      return compare_ints(x.year, this->year);
    }
    else if (this->month != x.month)
    {
      return compare_ints(x.month, this->month);
    }
    else if (this->day != x.day)
    {
      return compare_ints(x.day, this->day);
    }
    else if (this->hour != x.hour)
    {
      return compare_ints(x.hour, this->hour);
    }
    else if (this->minute != x.minute)
    {
      return compare_ints(x.minute, this->minute);
    }
    else if (this->second != x.second)
    {
      return (1000 * (x.second - this->second));
    }
    else
      return 0;
  }
  friend std::ostream &operator<<(std::ostream &os,
                                  const CTimeStamp &x)
  {
    os << std::setfill('0') << std::setw(4) << x.year << "-" << std::setw(2) << x.month
       << "-" << std::setw(2) << x.day
       << " " << std::setw(2) << x.hour
       << ":" << std::setw(2) << x.minute
       << ":" << std::setw(6) << std::fixed << std::setprecision(3) << x.second;
    return os;
  }
};
class CMail
{
private:
  CTimeStamp time;
  std::string f;
  std::string t;
  std::optional<std::string> sub;

public:
  CMail(const CTimeStamp &timeStamp,
        const std::string &from,
        const std::string &to,
        const std::optional<std::string> &subject) : time(timeStamp), f(from), t(to), sub(subject) {}
  int compareByTime(const CTimeStamp &x) const
  {
    CTimeStamp a = this->timeStamp();
    CTimeStamp b = x;
    return (a.compare(b));
  }
  int compareByTime(const CMail &x) const
  {
    return (this->time.compare(x.time));
  }
  const std::string &from() const
  {
    return this->f;
  }
  const std::string &to() const
  {
    return this->t;
  }
  const std::optional<std::string> &subject() const
  {
    return this->sub;
  }
  const CTimeStamp &timeStamp() const
  {
    return this->time;
  }
  friend std::ostream &operator<<(std::ostream &os,
                                  const CMail &x)
  {
    os << x.timeStamp() << " " << x.from() << " -> " << x.to();
    if (x.subject().has_value())
      os << ", subject: " << x.subject().value();
    return os;
  }
};
// your code will be compiled in a separate namespace
namespace MysteriousNamespace
{
#endif /* __PROGTEST__ */
  //----------------------------------------------------------------------------------------
  int get_month(std::string name)
  {
    std::vector<std::string> months = {"Jan", "Feb", "Mar",
                                       "Apr", "May", "Jun",
                                       "Jul", "Aug", "Sep",
                                       "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12; i++)
    {
      if (name == months[i])
        return (i + 1);
    }
    return 13;
  }
  static bool compare(const CMail &a, const CMail &b)
  {
    return a.compareByTime(b) > 0;
  }
  
  class CMailLog
  {
  public:
    int parseLog(std::istream &in)
    {
      std::map<std::string, std::string> logs_from;
      std::map<std::string, std::string> logs_to;
      std::map<std::string, std::optional<std::string>> logs_subject;
      for (std::string line; std::getline(in, line, '\n');)
      {
        if (line.length() < 27) continue;
        else {
        try
        {
                  
        std::string id = line.substr(line.find(':', 24) - 12, 12);
        if (line.find("from=") != std::string::npos)
        {
          logs_from[id] = line.substr(line.find("from=") + 5);
        }
        else if (line.find("subject=") != std::string::npos)
        {
          logs_subject[id] = line.substr(line.find("subject=") + 8);
        }
        else if (line.find("to=") != std::string::npos)
        {
          logs_to[id] = line.substr(line.find("to=") + 3);
          CTimeStamp newstamp(std::stoi(line.substr(7, 4)),
                              get_month(line.substr(0, 3)),
                              std::stoi(line.substr(4, 2)),
                              std::stoi(line.substr(12, 2)),
                              std::stoi(line.substr(15, 2)),
                              std::stod(line.substr(18, 6)));
          logs_subject.insert(std::pair<std::string, std::optional<std::string>>(id, std::nullopt));

          CMail newmail(newstamp,
                        logs_from.at(id),
                        logs_to.at(id),
                        logs_subject.at(id));
          auto iter = std::upper_bound(data.begin(), data.end(), newmail, compare);
          data.insert(iter, newmail);
        }
        }
        catch(...)
        {
          continue;
        }

      }
        /*

        */
      }
      return data.size();
    }
    std::list<CMail> listMail(const CTimeStamp &from,
                              const CTimeStamp &to) const
    {
      CMail mailfrom(from, "dummy", "dummy", std::nullopt);
    CMail mailto(to, "dummy", "dummy", std::nullopt);
    
    auto iter_from = std::lower_bound(data.begin(), data.end(), mailfrom, compare);
    auto iter_to = std::upper_bound(data.begin(), data.end(), mailto, compare);

    // Critical fix: Check if iterators are in valid order
    if (iter_from >= iter_to) {
        return {};
    }

    return std::list<CMail>(iter_from, iter_to);
}

    std::set<std::string> activeUsers(const CTimeStamp &from,
                                      const CTimeStamp &to) const
    {
      std::set<std::string> result;
      CMail mailfrom(from, "fromit", "fromit", "fromit");
      CMail mailto(to, "toit", "toit", "toit");
      auto iter_from = std::lower_bound(data.begin(), data.end(), mailfrom, compare);
      auto iter_to = std::upper_bound(data.begin(), data.end(), mailto, compare);

      while (iter_from != iter_to)
      {
        result.insert((*iter_from).from());
        result.insert((*iter_from).to());
        iter_from++;
      }
      return result;
    }

  private:
    std::vector<CMail> data;
  };
//----------------------------------------------------------------------------------------
#ifndef __PROGTEST__
} // namespace

std::string printMail(const std::list<CMail> &all)
{
  std::ostringstream oss;
  for (const auto &mail : all)
  {
    //std::cout << mail << std::endl;
    oss << mail << "\n";
  }
  return oss.str();
}

int main()
{
  std::cout << "zijem" << std::endl;

  MysteriousNamespace::CMailLog m;
  std::list<CMail> mailList;
  std::set<std::string> users;
  std::istringstream iss;

  iss.clear();
  iss.str(
      "Mar 29 2025 12:35:32.23\n"
      "Mar 29 2025 12:37:16.234 relay.fit.cvut.cz JlMSRW4232Df: from=person3@fit.cvut.cz\n"
      "Mar 29 2025 12:55:13.023 relay.fit.cvut.cz JlMSRW4232Df: subject=New progtest homework!\n"
      "Mar 29 2025 13:38:45.043 relay.fit.cvut.cz Kbced342sdgA: from=office13@fit.cvut.cz\n"
      "Mar 29 2025 13:36:13.023 relay.fit.cvut.cz JlMSRW4232Df: to=user76@fit.cvut.cz\n"
      "Mar 29 2025 13:55:31.456 relay.fit.cvut.cz KhdfEjkl247D: from=PR-department@fit.cvut.cz\n"
      "Mar 29 2025 14:18:12.654 relay.fit.cvut.cz Kbced342sdgA: to=boss13@fit.cvut.cz\n"
      "Mar 29 2025 14:48:32.563 relay.fit.cvut.cz KhdfEjkl247D: subject=Business partner\n"
      "Mar 29 2025 14:58:32.000 relay.fit.cvut.cz KhdfEjkl247D: to=HR-department@fit.cvut.cz\n"
      "Mar 29 2025 14:25:23.233 relay.fit.cvut.cz ADFger72343D: mail undeliverable\n"
      "Mar 29 2025 15:02:34.231 relay.fit.cvut.cz KhdfEjkl247D: to=CIO@fit.cvut.cz\n"
      "Mar 29 2025 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=CEO@fit.cvut.cz\n"
      "Mar 29 2025 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=dean@fit.cvut.cz\n"
      "Mar 29 2025 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=vice-dean@fit.cvut.cz\n"
      "Mar 29 2025 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=archive@fit.cvut.cz\n");
  assert(m.parseLog(iss) == 8);
  mailList = m.listMail(CTimeStamp(2025, 3, 28, 0, 0, 0),
                        CTimeStamp(2025, 3, 29, 23, 59, 59));
  assert(printMail(mailList) == R"###(2025-03-29 13:36:13.023 person3@fit.cvut.cz -> user76@fit.cvut.cz, subject: New progtest homework!
2025-03-29 14:18:12.654 office13@fit.cvut.cz -> boss13@fit.cvut.cz
2025-03-29 14:58:32.000 PR-department@fit.cvut.cz -> HR-department@fit.cvut.cz, subject: Business partner
2025-03-29 15:02:34.230 PR-department@fit.cvut.cz -> CEO@fit.cvut.cz, subject: Business partner
2025-03-29 15:02:34.230 PR-department@fit.cvut.cz -> dean@fit.cvut.cz, subject: Business partner
2025-03-29 15:02:34.230 PR-department@fit.cvut.cz -> vice-dean@fit.cvut.cz, subject: Business partner
2025-03-29 15:02:34.230 PR-department@fit.cvut.cz -> archive@fit.cvut.cz, subject: Business partner
2025-03-29 15:02:34.231 PR-department@fit.cvut.cz -> CIO@fit.cvut.cz, subject: Business partner
)###");
  mailList = m.listMail(CTimeStamp(2025, 3, 28, 0, 0, 0),
                        CTimeStamp(2025, 3, 29, 14, 58, 32));
  // std::cout << std::endl << std::endl;
  assert(printMail(mailList) == R"###(2025-03-29 13:36:13.023 person3@fit.cvut.cz -> user76@fit.cvut.cz, subject: New progtest homework!
2025-03-29 14:18:12.654 office13@fit.cvut.cz -> boss13@fit.cvut.cz
2025-03-29 14:58:32.000 PR-department@fit.cvut.cz -> HR-department@fit.cvut.cz, subject: Business partner
)###");
  mailList = m.listMail(CTimeStamp(2025, 3, 30, 0, 0, 0),
                        CTimeStamp(2025, 3, 30, 23, 59, 59));
  assert(printMail(mailList) == "");
  users = m.activeUsers(CTimeStamp(2025, 3, 28, 0, 0, 0),
                        CTimeStamp(2025, 3, 29, 23, 59, 59));
  assert(users == std::set<std::string>({"CEO@fit.cvut.cz", "CIO@fit.cvut.cz", "HR-department@fit.cvut.cz", "PR-department@fit.cvut.cz", "archive@fit.cvut.cz", "boss13@fit.cvut.cz", "dean@fit.cvut.cz", "office13@fit.cvut.cz", "person3@fit.cvut.cz", "user76@fit.cvut.cz", "vice-dean@fit.cvut.cz"}));
  users = m.activeUsers(CTimeStamp(2025, 3, 28, 0, 0, 0),
                        CTimeStamp(2025, 3, 29, 13, 59, 59));
  assert(users == std::set<std::string>({"person3@fit.cvut.cz", "user76@fit.cvut.cz"}));
  std::cout << "koncim" << std::endl;
  return EXIT_SUCCESS;
  /*
   */
}
#endif /* __PROGTEST__ */
