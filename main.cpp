#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <vector>

class Personal_Inf {
private:

	std::string name;
	std::string last_name;
	std::string email;
	std::string number;
	std::string name_id;

	pqxx::connection base;

public:
	void create_table() {
		pqxx::work txn(base);
		std::string createTableQuery = "CREATE TABLE IF NOT EXISTS client ("
			"id SERIAL PRIMARY KEY,"
			"name VARCHAR(100) NOT NULL,"
			"last_name VARCHAR(100) NOT NULL,"
			"email VARCHAR(100) NOT NULL"
			")";
		txn.exec(createTableQuery);
		txn.commit();

		pqxx::work pxn(base);
		std::string createTableQuery1 = "CREATE TABLE IF NOT EXISTS number ("
			"id SERIAL PRIMARY KEY,"
			"number VARCHAR(100) NOT NULL,"
			"name_id INT NOT NULL references client(id)"
			")";
		pxn.exec(createTableQuery1);
		pxn.commit();
	}

	void insert_client() {
		std::cout << "¬ведите им€ клиента:";
		std::cin >> name;
		std::cout << "¬ведите фамилию клиента:";
		std::cin >> last_name;
		std::cout << "¬ведите email клиента:";
		std::cin >> email;

		pqxx::work tx(base);
		tx.exec("INSERT INTO client(name,last_name,email) "
			"VALUES('" + tx.esc(name) + "', '" + tx.esc(last_name) + "', '" + tx.esc(email) + "');");
		tx.commit();
	}

	void insert_client_number() {
		try {
			std::cout << "¬ведите номер телефона:";
			std::cin >> number;
			std::cout << "¬ведите id сотрудника:";
			std::cin >> name_id;

			pqxx::work pt(base);
			pt.exec("INSERT INTO number(number,name_id) "
				"VALUES('" + pt.esc(number) + "', '" + pt.esc(name_id) + "');");
			pt.commit();
		}
		catch (pqxx::sql_error e) {
			std::cout << e.what() << std::endl;
		}
	}

	void update_inf() {
		pqxx::work ty(base);
		unsigned int choice;
		std::string id;
		std::string change;

		std::cout << "„то хотите изменить:"
			"1.»м€  "
			"2.‘амилию  "
			"3.Email  "
			"4.Ќомер телефона  " << std::endl;
		std::cin >> choice;

		try {
			std::cout << "¬ведите id сотрудника:";
			std::cin >> id;
			switch (choice) {
			case 1:std::cout << "¬ведите им€:"; std::cin >> change; ty.exec("UPDATE client SET name = " + ty.esc(change) + " where id=" + id); break;
			case 2:std::cout << "¬ведите фамилию:"; std::cin >> change; ty.exec("UPDATE client SET last_name = " + ty.esc(change) + " where id=" + id); break;
			case 3:std::cout << "¬ведите email:"; std::cin >> change; ty.exec("UPDATE client SET email = " + ty.esc(change) + " where id=" + id); break;
			case 4:std::cout << "¬ведите номер телефона:"; std::cin >> change; ty.exec("UPDATE number SET number = " + ty.esc(change) + " where name_id=" + id); break;
			default:break;
			}
			ty.commit();
		}
		catch (pqxx::sql_error e) {
			std::cout << e.what() << std::endl;
		}
	}

	void delete_number() {
		try {
			std::string id;
			std::cout << "¬ведите id сотрудника, чей телефон вы хотите удалить:";
			std::cin >> id;

			pqxx::work wt(base);
			wt.exec("DELETE FROM number WHERE name_id = " + id);
			wt.commit();

		}
		catch (pqxx::sql_error e) {
			std::cout << e.what() << std::endl;
		}
	}

	void delete_client() {
		try {
			std::string id;
			std::cout << "¬ведите id сотрудника:";
			std::cin >> id;

			pqxx::work wt(base);
			wt.exec("DELETE FROM number WHERE name_id = " + id);
			wt.exec("DELETE FROM client WHERE id = " + id);
			wt.commit();

		}
		catch (pqxx::sql_error e) {
			std::cout << e.what() << std::endl;
		}
	}

	void search_client() {
		try {
			std::vector<std::string>data;
			std::string info;

			std::cout << "¬ведите им€ клиента, если непомните, то введите любой символ:";
			std::cin >> info;
			data.push_back(info);

			std::cout << "¬ведите фамилию клиента, если непомните, то введите любой символ:";
			std::cin >> info;
			data.push_back(info);

			std::cout << "¬ведите email клиента, если непомните, то введите любой символ:";
			std::cin >> info;
			data.push_back(info);

			std::cout << "¬ведите телефон клиента, если непомните, то введите любой символ:";
			std::cin >> info;
			data.push_back(info);

			std::string query = "SELECT *, number.number FROM client  INNER JOIN number ON client.id = number.name_id WHERE "
				"name = '" + data[0] + "' OR last_name = '" + data[1] + "' OR email ='" + data[2] + 
				"' OR number ='" + data[3] + "';";

			pqxx::work wt(base);
			pqxx::result result = wt.exec(query);
			for (pqxx::result::const_iterator row = result.begin(); row != result.end(); ++row) {
				std::cout << "Client ID: " << row["id"].as<int>() << ", Name: " <<
					row["name"].as<std::string>() << ", LastName: " << row["last_name"].as<std::string>() <<
					", Email:" << row["email"].as<std::string>()<<", Number:"<<row["number"].as<std::string>()<< std::endl;
			}
			wt.commit();
		}
		catch (pqxx::sql_error e) {
			std::cout << e.what() << std::endl;
		}
	}


	Personal_Inf() :base("host=localhost port = 5432 dbname = personal_info user = postgres password = 13Afudoh@@") {}
};



int main() {

	setlocale(LC_ALL, "Russian");

	Personal_Inf man;
	//man.create_table();
	//man.insert_client();
	//man.insert_client_number();
	//man.update_inf();
	//man.delete_number();
	//man.delete_client();
	//man.search_client();

	return 0;
}





