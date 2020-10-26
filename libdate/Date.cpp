#include "Date.hpp"

date::Date::Date()
{
	m_usable = false;
}

date::Date::Date(int day, int month, int year, bool month_as_index = false)
{
	m_day = day;
	if(month_as_index)
	{
        // Month passed is from 0-11
		m_month = month + 1;
	}
	else
	{
        // Month passed is from 1-12
		m_month = month;
	}
	m_year = year;
}

bool date::Date::Validate_Date()
{
	int month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	bool adjusted = false;
	bool validating = true;
	while(validating)
	{
		// Check for Leap Year
        month_days[1] = ((m_year % 4) == 0) ? 29 : 28;

		// Check if month if not in between 1-12
		while(m_month > 12)
		{
			m_month -= 12;
			++m_year;
			adjusted = true;
		}
		while(m_month < 1)
		{
			m_month += 12;
			--m_year;
			adjusted = true;
		}

		// Check days
		if(m_day > month_days[m_month - 1])
		{
			m_day -= month_days[m_month - 1];
            ++m_month;
			adjusted = true;
		}
		if(m_day < 1)
		{
			m_day *= -1;
			m_day = month_days[m_month - 1] - m_day;
            --m_month;
			adjusted = true;
		}

        // If the date is valid, set the validating flag to false to exit the loop
		if((m_month >= 1 && m_month <= 12) && (m_day >=1 && m_day <= month_days[m_month - 1]))
		{
			validating = false;
		}
	}
	return !adjusted; // Return True if no corrections were needed (Date was okay); False if corrections were made
}

int date::Date::Get_Day()
{
	return m_day;
}

std::string date::Date::Get_Month_Str(bool as_abbreviation)
{
	switch(m_month)
	{
		case 0:
			return((as_abbreviation) ? "Jan" : "January");
		case 1:
			return((as_abbreviation) ? "Feb" : "February");
		case 2:
			return((as_abbreviation) ? "Mar" : "March");
		case 3:
			return((as_abbreviation) ? "Apr" : "April");
		case 4:
			return((as_abbreviation) ? "May" : "May");
		case 5:
			return((as_abbreviation) ? "Jun" : "June");
		case 6:
			return((as_abbreviation) ? "Jul" : "July");
		case 7:
			return((as_abbreviation) ? "Aug" : "August");
		case 8:
			return((as_abbreviation) ? "Sept" : "September");
		case 9:
			return((as_abbreviation) ? "Oct" : "October");
		case 10:
			return((as_abbreviation) ? "Nov" : "November");
		case 11:
			return((as_abbreviation) ? "Dec" : "December");
		default:
			// Should never happen
			return(nullptr);
	}
}

int date::Date::Get_Month(bool month_as_index)
{
	if(month_as_index)
	{
		return m_month;
	}
	else
	{
		return m_month + 1;
	}
}

std::string date::Date::Get_Day_Name()
{
	if(m_usable)
	{
		int temp_year = m_year;
		int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
		temp_year -= m_month < 3;
		switch(((temp_year + temp_year/4 - temp_year/100 + temp_year/400 + t[m_month-1] + m_day) % 7))
		{
			case 0:
				return("Sunday");
			case 1:
				return("Monday");
			case 2:
				return("Tuesday");
			case 3:
				return("Wednesday");
			case 4:
				return("Thursday");
			case 5:
				return("Friday");
			case 6:
				return("Saturday");
			default:
				return(nullptr); // Should never get here, but if it does, return null
		}
	}
	return(nullptr); // Should never get here, but if it does, return null
}

int date::Date::Get_Day_Name(bool return_as_index)
{
	if(m_usable)
	{
		int temp_year = m_year;
		int result;
		int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
		temp_year -= m_month < 3;
		result = ((temp_year + temp_year/4 - temp_year/100 + temp_year/400 + t[m_month-1] + m_day) % 7);
		if(!return_as_index)
		{
			result++;
		}

		return(result);
	}
    return -1;
}

void date::Date::Add_Days(int days)
{
	m_day += days;
	this->Validate_Date();
}

std::tuple<int, int, int> date::Date::Get_Date()
{
	return std::make_tuple(m_day, m_month, m_year);
}

// Operators
void date::Date::operator++()
{
	m_day++;
	this->Validate_Date();
}

void date::Date::operator--()
{
	m_day--;
	this->Validate_Date();
}

void date::Date::operator+=(int days)
{
	m_day += days;
	this->Validate_Date();
}

void date::Date::operator-=(int days)
{
	m_day -= days;
	this->Validate_Date();
}

bool date::Date::operator!=(Date second_date)
{
	return((this->m_day != second_date.m_day) || (this->m_month != second_date.m_month) || (this->m_year != second_date.m_year));
}

bool date::Date::operator==(Date second_date)
{
	return((this->m_day == second_date.m_day) && (this->m_month == second_date.m_month) && (this->m_year == second_date.m_year));
}

bool date::Date::operator<(Date second_date)
{
	if(this->m_year < second_date.m_year)
	{
		return(true);
	}
	else if(this->m_year > second_date.m_year)
	{
		return(false);
	}

	// If here, the years have to be equal
	if(this->m_month < second_date.m_month)
	{
		return(true);
	}
	else if(this->m_month > second_date.m_month)
	{
		return(false);
	}

	// If here, the years and months are equal
	if(this->m_day < second_date.m_day)
	{
		return(true);
	}

	return(false);
}

bool date::Date::operator>(Date second_date)
{
	if(this->m_year > second_date.m_year)
	{
		return(true);
	}
	else if(this->m_year < second_date.m_year)
	{
		return(false);
	}

	// If here, the years have to be equal
	if(this->m_month > second_date.m_month)
	{
		return(true);
	}
	else if(this->m_month < second_date.m_month)
	{
		return(false);
	}

	// If here, the years and months are equal
	if(this->m_day > second_date.m_day)
	{
		return(true);
	}

	return(false);
}

bool date::Date::operator<=(Date second_date)
{
	if(this->m_year < second_date.m_year)
	{
		return(true);
	}
	else if(this->m_year > second_date.m_year)
	{
		return(false);
	}

	// If here, the years have to be equal
	if(this->m_month < second_date.m_month)
	{
		return(true);
	}
	else if(this->m_month > second_date.m_month)
	{
		return(false);
	}

	// If here, the years and months are equal
	if(this->m_day <= second_date.m_day)
	{
		return(true);
	}

	return(false);
}

bool date::Date::operator>=(Date second_date)
{
	if(this->m_year > second_date.m_year)
	{
		return(true);
	}
	else if(this->m_year < second_date.m_year)
	{
		return(false);
	}

	// If here, the years have to be equal
	if(this->m_month > second_date.m_month)
	{
		return(true);
	}
	else if(this->m_month < second_date.m_month)
	{
		return(false);
	}

	// If here, the years and months are equal
	if(this->m_day >= second_date.m_day)
	{
		return(true);
	}

	return(false);
}

std::string date::Date::ToString(date::DateFormat style, char delimiter=0)
{
	if(m_usable)
	{
        std::string str_year;
        char* formatted_date;
		switch(style)
		{
            case date::DateFormat::MMDDYY:
    			str_year = std::to_string(m_year);
                std::sprintf(formatted_date, "%02d%c%02d%c%c%c", m_month, delimiter, m_day, delimiter, str_year[2], str_year[3]);
                break;
            case date::DateFormat::MMDDYYYY:
                std::sprintf(formatted_date, "%02d%c%02d%c%d", m_month, delimiter, m_day, delimiter, m_year);
                break;
            case date::DateFormat::DDMMYY:
                 str_year = std::to_string(m_year);
                 std::sprintf(formatted_date, "%02d%c%02d%c%c%c", m_day, delimiter, m_month, delimiter, str_year[2], str_year[3]);
            case date::DateFormat::DDMMYYYY:
                 std::sprintf(formatted_date, "%02d%c%02d%c%d", m_day, delimiter, m_month, delimiter, m_year);
            case date::DateFormat::MDYYYY:
                 std::sprintf(formatted_date, "%d%c%d%c%d", m_month, delimiter, m_day, delimiter, m_year);
            case date::DateFormat::MDYY:
				 str_year = std::to_string(m_year);
				 sprintf(formatted_date, "%d%c%d%c%c%c", m_month, delimiter, m_day, delimiter, str_year[2], str_year[3]);
            case date::DateFormat::DMYYYY:
                 std::sprintf(formatted_date, "%d%c%d%c%d", m_day, delimiter, m_month, delimiter, m_year);
            case date::DateFormat::DMYY:
				str_year = std::to_string(m_year);
                std::sprintf(formatted_date, "%d%c%d%c%c%c", m_day, delimiter, m_month, delimiter, str_year[2], str_year[3]);
			default:
				// As this is based off of an enumeration, this should never happen:
				return nullptr;
		}
        return formatted_date;
	}
    return nullptr;
}
