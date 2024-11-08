#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
constexpr unsigned DOW_MON      = 0b00000001;
constexpr unsigned DOW_TUE      = 0b00000010;
constexpr unsigned DOW_WED      = 0b00000100;
constexpr unsigned DOW_THU      = 0b00001000;
constexpr unsigned DOW_FRI      = 0b00010000;
constexpr unsigned DOW_SAT      = 0b00100000;
constexpr unsigned DOW_SUN      = 0b01000000;
constexpr unsigned DOW_WORKDAYS = DOW_MON | DOW_TUE | DOW_WED | DOW_THU | DOW_FRI;
constexpr unsigned DOW_WEEKEND  = DOW_SAT | DOW_SUN;
constexpr unsigned DOW_ALL      = DOW_WORKDAYS | DOW_WEEKEND;

typedef struct TDate
{
  unsigned m_Year;
  unsigned m_Month;
  unsigned m_Day;
} TDATE;

TDATE makeDate ( unsigned y,
                 unsigned m,
                 unsigned d )
{
  TDATE res = { y, m, d };
  return res;
}
#endif /* __PROGTEST__ */

void print(TDATE date)
{
    printf("%u %u %u", date.m_Day, date.m_Month, date.m_Year);
}
int exp(int n){
    int result = 1;
    for (int i = 0; i<n; i++){
        result *=2;
    }
    return result;
}

int round_up_division (unsigned a, unsigned b)
{
if (a%b == 0)
{
    return a/b;
} else
{
    return (a/b)+1;
}

}

int check_if_leap(TDATE date)
{
    unsigned year = date.m_Year; // 1 if leap 0 if not
    if (year % 4000 == 0){
        return 0;
    } else if (year % 400 == 0){
        return 1;
    } else if (year % 100 == 0){
        return 0;
    } else if (year % 4 == 0) {
        return 1;
    } else {
        return 0;
    }
}
TDATE month_incrementer  (TDATE date)
{
    date.m_Month +=1;
    if (date.m_Month <=12)
    {
        return date;
    } else
    {
        date.m_Month =1;
        date.m_Year +=1;
        return date;
    }
}
int get_day_of_week(TDATE date)
{
    int weekday = 2;
    TDATE current = makeDate(2000, 1, 1);
    int months_leap[12] = {3, 1, 3, 2, 3, 2, 3, 3, 2, 3, 2, 3};
    int months[12] = {3, 0, 3, 2, 3, 2, 3, 3, 2, 3, 2, 3};
    int active = 1;
    while (active)
    {
        if(!check_if_leap(current))
        {
            weekday = (weekday + months[(current.m_Month+10)%12])%7;
        } else {
            weekday = (weekday + months_leap[(current.m_Month+10)%12])%7;
        }
        //print(current);
        //printf(" %d ty den v tyzdni\n", weekday);
        if ((current.m_Month == date.m_Month && current.m_Year == date.m_Year))
        {
            weekday = (weekday +date.m_Day-1)%7;
            active = 0;
        } else
        {
            current = month_incrementer(current);
        }
        
    }
    return weekday;
}
int check_valid_date(TDATE date)
{
    if (date.m_Month == 0 || date.m_Day  == 0) {
        return 0;
    } else if (date.m_Month >12 || date.m_Day > 31){
        return 0;
    } else if (((date.m_Month%2 == 0 && date.m_Month <7) && date.m_Day  == 31) || ((date.m_Month%2 == 1 && date.m_Month > 8) && date.m_Day  == 31)){
        return 0;
    } else if (date.m_Month == 2 && check_if_leap(date) == 1 && date.m_Day  > 29){
        return 0;
    } else if (date.m_Month == 2 && check_if_leap(date) == 0 && date.m_Day > 28 ) {
        return 0;
    } else {
        return 1;
    }

}
TDATE incrementer(TDATE date, int number_of_days) //doesnt work for values bigger than idk maybe 28
{
    int months_leap[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    date.m_Day +=number_of_days;
    if (check_valid_date(date))
    {
        return date;
    }
    if(check_if_leap(date))
    {
        date.m_Day -= months_leap[date.m_Month-1];
        date.m_Month +=1;
    } else
    {
        date.m_Day -= months[date.m_Month-1];
        date.m_Month +=1;
    }
    if (check_valid_date(date))
    {
        return date;
    }
    date.m_Month = 1;
    date.m_Year +=1;
    //printf("   %u     \n", date.m_Year);
    if (check_valid_date(date))
    {
        return date;
    } else
    {
        return makeDate(0000, 0, 0);
    }
    
}

int * get_number_of_connections(int per_work_day,int mask)
{
    static int array[7];
    for (int i = 0; i<7; i++)
    {
        if (mask & exp(i))
        {
            if (i<5)
            {
                array[i] = per_work_day;
            } else if (i == 5)
            {
                array[i] = round_up_division(per_work_day, 2);
            } else
            {
                array[i] = round_up_division(per_work_day, 3);
            }
        } else
        {
            array[i] = 0;
        }
    }
    return array;
} 

int check_if_valid_date_range(TDATE a, TDATE b)
{
    if (a.m_Year > b.m_Year)
    {
        return 0;
    } else if(a.m_Month > b.m_Month && a.m_Year == b.m_Year)
    {
        return 0;
    } else if (a.m_Day > b.m_Day && a.m_Month == b.m_Month && a.m_Year == b.m_Year)
    {
        return 0;
    } else
    {
        return 1;
    }
}
typedef struct DateCount
{
    long long count_weeks;
    TDATE date;
} Datecount;

Datecount get_weeks_between_dates (TDATE start, TDATE end)
{
    long long count_weeks = 0;
    TDATE current = start;
    struct DateCount result;
    int active = 1;
    while (active)
    {
        if(!check_if_valid_date_range(incrementer(current,7), end))
        {
            active =0;
            break;
        } else
        {
            count_weeks++;
            current = incrementer(current, 7);
        }
        //print(current);
        //printf("  %lld tyzdnov ubehlo\n", count_weeks);
        

    }

    result.count_weeks = count_weeks;
    result.date = current;
    return result;
}
int array_sum(int array[], int size)
{
    int sum = 0;
    for (int i = 0; i<size; i++)
    {
        sum += array[i];
    }
    return sum;
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("Element %d: %d\n", i, arr[i]);
    }
}

long long countConnections ( TDATE     from,
                             TDATE     to,
                             unsigned  perWorkDay,
                             unsigned  dowMask )
{
/*
    printf("\n\n\n---------------nove volanie---------------\n");
    print(from);
    printf(" do ");
    print(to);
    printf("  denne %d a maska %u\n", perWorkDay, dowMask);
    */
    if (check_if_valid_date_range(from, to)== 0 || check_valid_date(from) == 0 || check_valid_date(to) == 0)
    {
        //printf("dropujem kvoli %d %d %d\n", check_if_valid_date_range(from, to),check_valid_date(from), check_valid_date(to));
        return -1;
    }
    int day_of_week = get_day_of_week(from);
    long long count = 0;
    int working_mask = dowMask;
    struct DateCount count_weeks = get_weeks_between_dates(from, to);
    int * number_of_connections = get_number_of_connections(perWorkDay, working_mask);
    //printArray(number_of_connections, 7);
    int per_week = array_sum(number_of_connections, 7);
    //printf("kazdy tyzden to je %d preco to nejde\n", per_week);
    count += per_week * count_weeks.count_weeks;
    TDATE current = count_weeks.date;
    //print(current);
    //printf("  po %lld tyzdnoch to je %lld a je to tento den %d \n", count_weeks.count_weeks, count, day_of_week);
    while (1)
    {
        count += number_of_connections[day_of_week];
        //printf("den %d a je to %lld  ", day_of_week, count);
        //print(current);
        //printf("\n");
        if ((current.m_Day == to.m_Day && current.m_Month == to.m_Month && current.m_Year == to.m_Year))
        {
            //print(current);
            //printf("koncim s finalnym poctom %lld \n", count);
            break;
        } else
        {
            //print(current);
            //printf(" a pokracujem\n");
            day_of_week = (day_of_week +1)%7;
            current = incrementer(current, 1);
        }
    }
    return count;
}

int minimum_from_array (int array[], int size)
{
    int minimum = 9999999;
    for (int i = 0; i<size; i++)
    {
        if (array[i]<minimum)
        {
            minimum = array[i];
        }
    }
    return minimum;
}
TDATE     endDate          ( TDATE     from,
                             long long connections,
                             unsigned  perWorkDay,
                             unsigned  dowMask )
{
    
    TDATE current = from;
    int working_mask = dowMask;
    //printf("\n\n\n\n--------------nove volanie endDate-------------\n");
    if(check_valid_date(from) == 0 || connections <0 || perWorkDay == 0 || working_mask == 0)
    {  
        //printf("dropujem lebo %d %lld %d %d\n", check_valid_date(from), connections, perWorkDay, working_mask);
        return makeDate(0000,0,0);
    }
    int day_of_week = get_day_of_week(from);
    long long connections_left= connections;
    int * number_of_connections = get_number_of_connections(perWorkDay, working_mask);
    int per_week = array_sum(number_of_connections, 7);
    int minimum = minimum_from_array(number_of_connections, 7);
    //printf("per week %d a minimum %d\n", per_week, minimum);
    //printf("working mask %d \n", working_mask);

    if(connections < number_of_connections[day_of_week])
    {
        return makeDate(0000,0,0);
    }

    while (connections_left > per_week)
    {
        //printf("tocim sa %lld\n", connections_left);
        connections_left-=per_week;
        current = incrementer(current, 7);
    }
    //printf("po week section ostava %lld\n", connections_left);
    int active = 1;
    //printf("pred while loopom\n");
    while(active)
    {
        //printf("vo while loope\n");
        connections_left -= number_of_connections[day_of_week];
        //print(current);
        //printf(" je %d den a po jeho odpocitani mi ostava %lld\n", day_of_week, connections_left);
        if(connections_left< minimum || connections_left -number_of_connections[(day_of_week+1)%7]<0)
        {
            //print(current);
            //printf(" a koncim \n");
            active = 0;
        } else
        {
            current = incrementer(current,1);
            day_of_week = (day_of_week +1)%7;
            //print(current); 
            //printf(" po odcitani a zostatok je %lld\n", connections_left);
        }

    }
    return current;
}

#ifndef __PROGTEST__

void testiky()
{
    TDATE d;
    assert ( countConnections ( makeDate ( 2024, 10, 1 ), makeDate ( 2024, 10, 31 ), 1, DOW_ALL ) == 31 );
    assert ( countConnections ( makeDate ( 2024, 10, 1 ), makeDate ( 2024, 10, 31 ), 10, DOW_ALL ) == 266 );
    assert ( countConnections ( makeDate ( 2024, 10, 1 ), makeDate ( 2024, 10, 31 ), 1, DOW_WED ) == 5 );
    assert ( countConnections ( makeDate ( 2024, 10, 2 ), makeDate ( 2024, 10, 30 ), 1, DOW_WED ) == 5 );
    assert ( countConnections ( makeDate ( 2024, 10, 1 ), makeDate ( 2024, 10, 1 ), 10, DOW_TUE ) == 10 );
    assert ( countConnections ( makeDate ( 2024, 10, 1 ), makeDate ( 2024, 10, 1 ), 10, DOW_WED ) == 0 );
    assert ( countConnections ( makeDate ( 2024, 1, 1 ), makeDate ( 2034, 12, 31 ), 5, DOW_MON | DOW_FRI | DOW_SAT ) == 7462 );
    assert ( countConnections ( makeDate ( 2024, 1, 1 ), makeDate ( 2034, 12, 31 ), 0, DOW_MON | DOW_FRI | DOW_SAT ) == 0 );
    assert ( countConnections ( makeDate ( 2024, 1, 1 ), makeDate ( 2034, 12, 31 ), 100, 0 ) == 0 );
    assert ( countConnections ( makeDate ( 2024, 10, 10 ), makeDate ( 2024, 10, 9 ), 1, DOW_MON ) == -1 );
    assert ( countConnections ( makeDate ( 2024, 2, 29 ), makeDate ( 2024, 2, 29 ), 1, DOW_ALL ) == 1 );
    assert ( countConnections ( makeDate ( 2023, 2, 29 ), makeDate ( 2023, 2, 29 ), 1, DOW_ALL ) == -1 );
    assert ( countConnections ( makeDate ( 2100, 2, 29 ), makeDate ( 2100, 2, 29 ), 1, DOW_ALL ) == -1 );
    assert ( countConnections ( makeDate ( 2400, 2, 29 ), makeDate ( 2400, 2, 29 ), 1, DOW_ALL ) == 1 );
    assert ( countConnections ( makeDate ( 4000, 2, 29 ), makeDate ( 4000, 2, 29 ), 1, DOW_ALL ) == -1 );

    d = endDate ( makeDate ( 2024, 10, 1 ), 100, 1, DOW_ALL );
    assert ( d . m_Year == 2025 && d . m_Month == 1 && d . m_Day == 8 );
    d = endDate ( makeDate ( 2024, 10, 1 ), 100, 6, DOW_ALL );
    assert ( d . m_Year == 2024 && d . m_Month == 10 && d . m_Day == 20 );
    d = endDate ( makeDate ( 2024, 10, 1 ), 100, 1, DOW_WORKDAYS );
    assert ( d . m_Year == 2025 && d . m_Month == 2 && d . m_Day == 17 );
    d = endDate ( makeDate ( 2024, 10, 1 ), 100, 4, DOW_WORKDAYS );
    assert ( d . m_Year == 2024 && d . m_Month == 11 && d . m_Day == 4 );
    d = endDate ( makeDate ( 2024, 10, 1 ), 100, 1, DOW_THU );
    assert ( d . m_Year == 2026 && d . m_Month == 9 && d . m_Day == 2 );
    d = endDate ( makeDate ( 2024, 10, 1 ), 100, 2, DOW_THU );
    assert ( d . m_Year == 2025 && d . m_Month == 9 && d . m_Day == 17 );
    d = endDate ( makeDate ( 2024, 10, 1 ), 100, 0, DOW_THU );
    assert ( d . m_Year == 0 && d . m_Month == 0 && d . m_Day == 0 );
    d = endDate ( makeDate ( 2024, 10, 1 ), 100, 1, 0 );
    assert ( d . m_Year == 0 && d . m_Month == 0 && d . m_Day == 0 );



}
    


int main ()
{
    //TDATE datum1, datum2;
    //datum1 = makeDate ( 2000, 1, 1);
    //datum2 = makeDate ( 1000000, 1, 1);
    //int n =5;
    //printf("kolky den je 30.10.2024 %d\n", get_day_of_week(makeDate(2024,10,30)));
    //printf("dnesok je valid %d a je priestupny %d a je to tento den tyzdna %d\n", check_valid_date(today), check_if_leap(today), get_day_of_week(today));
    //printf("dnesok je %u %u %u a zajtrajsok je %u %u %u \n", today.m_Day, today.m_Month, today.m_Year, incrementer(today).m_Day, incrementer(today).m_Month, incrementer(today).m_Year);
    //printf("dokopy je %lld spojov \n", countConnections(datum1, datum2, n ,DOW_ALL));
    //incrementer_tester();

    testiky();

}
#endif /* __PROGTEST__ */
