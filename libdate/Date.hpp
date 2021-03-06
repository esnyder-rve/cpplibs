/*
 * TODO
 *     Fix month implementation from 0-11 to 1-12
 *     Change function names
 *     Add abbreviations to Get_Day_Name
 *     Remove std::tuple
 *     Add export to std::chrono
 */
#ifndef DATE_HPP
#define DATE_HPP

#include <tuple>
#include <string>
#include <cstdio>
#include <chrono> // <-- TODO

namespace date
{
    enum class DateFormat : short int
    {
        MMDDYY,
        MMDDYYYY,
        DDMMYY,
        DDMMYYYY,
        MDYYYY,
        MDYY,
        DMYYYY,
        DMYY
    };

    class Date
    {
        private:
            int m_day;
            int m_month;        // Month of year stored as 1-12
            int m_year;
            bool m_usable;

        public:
            // Functions:
            /**
             * \brief  Create an empty date object
             *
             * \detail Creates an empty date object. An internal flag gets set that
             *         makes the object unusable in many regards.
             */
            Date();

            /**
             * \brief  Create and initialize a date object.
             *
             * \detail Creates a date object with a series of integer values for the
             *         day, month, and year. A boolean flag is required to set whether
             *         or not the month entered is an index (from 0-11) or actual
             *         month (1-12)
             *
             * \param day   The day of the month from 1-31
             *
             * \param month The month of the year (see month_as_index)
             *
             * \param year  The year as a four-digit integer
             *
             * \param month_as_index A boolean flag to tell whether or not the number
             *              is an index (0-11) or actual number (1-12).
             */
            Date(int day, int month, int year, bool month_as_index);
            int Day();
            void Day(int day);
            int Month();
            void Month(int month, bool month_as_index = false);
            int Year();
            void Year(int year);
            bool Validate();
            std::string MonthName(bool);
            std::string DayName(bool);
            int DayOfWeek();
            void AddDays(int);
            std::string ToString(DateFormat, char);
            std::string ToString(const char*);
            std::string ToString(std::string);
            // Operators
            void operator++();
            void operator--();
            void operator++(int);
            void operator--(int);
            void operator+=(int);
            void operator-=(int);
            bool operator==(Date);
            bool operator!=(Date);
            bool operator<(Date);
            bool operator>(Date);
            bool operator<=(Date);
            bool operator>=(Date);
    };
}
#endif // DATE_HPP
