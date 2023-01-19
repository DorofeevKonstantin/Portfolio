#include <iostream>
#include <string>
#include <conio.h>

#include "Record.hpp"
#include "Tables.hpp"

int main()
{
	std::string whyDoesItMakeSense; // SSO,  class _String_val, field union _Bxty

	ITable<Record>* table = nullptr;
	std::string tableType;
	int menuChoise;
	bool exit = false;
	std::cout << "Static or dynamic ? (s / d) : " << std::endl;
	while (true)
	{
		std::cin >> tableType;
		if (tableType.starts_with("s"))
		{
			table = new TableStat<Record>;
			break;
		}
		else if (tableType.starts_with("d"))
		{
			table = new TableDyn<Record>;
			break;
		}
		else
			std::cout << "Error : wrong choise, try again :" << std::endl;
	}
	if (table != nullptr)
	{
		size_t id;
		std::string name;
		Record* newRecord, * finded = nullptr;
		while (!exit)
		{
			system("cls");
			table->print();
			std::cout << "Options: 1-add 2-find 3-delete(after find) 4-exit" << std::endl;
			std::cin >> menuChoise;
			switch (menuChoise)
			{
			case 1:
				std::cout << "Enter id and name : " << std::endl;
				std::cin >> id >> name;
				newRecord = new Record{ id,name };
				table->add(newRecord);
				delete newRecord;
				break;
			case 2:
				std::cout << "Enter id to find : " << std::endl;
				std::cin >> id;
				finded = table->find(id);
				if (finded != nullptr)
					std::cout << "Find : " << finded->name << std::endl;
				else
					std::cout << "Not finded" << std::endl;
				_getch();
				break;
			case 3:
				if (finded != nullptr)
				{
					table->del(finded);
					finded = nullptr;
				}
				break;
			case 4:
				exit = true;
				delete table;
				std::cout << "Exit." << std::endl;
				break;
			default:
				break;
			}
		}
	}
	system("pause");
	return 0;
}