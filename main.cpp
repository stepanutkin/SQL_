#include <iostream>
#include <pqxx/pqxx>

class ClientManager {
public:
    ClientManager() : connection("dbname=mydb user=myuser password=mypasswd") {}

    void initDbStructure() {
        pqxx::work txn(connection);
        txn.exec("CREATE TABLE IF NOT EXISTS clients (client_id SERIAL PRIMARY KEY, first_name TEXT, last_name TEXT, email TEXT)");
        txn.exec("CREATE TABLE IF NOT EXISTS phone_numbers (phone_id SERIAL PRIMARY KEY, client_id INT REFERENCES clients(client_id), phone_number TEXT)");
        txn.commit();
    }

    int addClient(const std::string& firstName, const std::string& lastName, const std::string& email) {
        int clientId = -1;
        pqxx::work txn(connection);

        pqxx::result result = txn.exec("INSERT INTO clients (first_name, last_name, email) VALUES ('" + firstName + "', '" + lastName + "', '" + email + "') RETURNING client_id");
        if (!result.empty()) {
            clientId = result[0][0].as<int>();
        }

        txn.commit();
        return clientId;
    }

    void addPhoneNumber(int clientId, const std::string& phoneNumber) {
        pqxx::work txn(connection);
        txn.exec("INSERT INTO phone_numbers (client_id, phone_number) VALUES (" + std::to_string(clientId) + ", '" + phoneNumber + "')");
        txn.commit();
    }

    void updateClient(int clientId, const std::string& firstName, const std::string& lastName, const std::string& email) {
        pqxx::work txn(connection);
        txn.exec("UPDATE clients SET first_name = '" + firstName + "', last_name = '" + lastName + "', email = '" + email + "' WHERE client_id = " +
            std::to_string(clientId));
        txn.commit();
    }

    void removeClient(int clientId) {
        pqxx::work txn(connection);
        txn.exec("DELETE FROM clients WHERE client_id = " + std::to_string(clientId));
        txn.exec("DELETE FROM phone_numbers WHERE client_id = " + std::to_string(clientId));
        txn.commit();
    }

    void findClient(const std::string& searchValue) {
        pqxx::work txn(connection);

        std::string query = "SELECT clients.client_id, clients.first_name, clients.last_name, clients.email, phone_numbers.phone_number "
            "FROM clients "
            "LEFT JOIN phone_numbers ON clients.client_id = phone_numbers.client_id WHERE clients.first_name = '"
            + searchValue + "' OR "
            "clients.last_name = '" + searchValue + "' OR "
            "clients.email = '" + searchValue + "' OR "
            "phone_numbers.phone_number = '" + searchValue + "'";

        pqxx::result result = txn.exec(query);

        for (pqxx::result::size_type i = 0; i < result.size(); ++i) {
            std::cout << "Employee ID: " << result.at(i)[0].as<int>() << ", Name: " << 
                result.at(i)[1].as<std::string>() << " " << 
                result.at(i)[2].as<std::string>() << std::endl;
        }

        txn.commit();
    }

private:
    pqxx::connection connection;
};



int main() {

    setlocale(LC_ALL, "russian");
    ClientManager clientManager;

    // Инициализация структуры БД
    clientManager.initDbStructure();

    // Добавление клиента
    int clientId = clientManager.addClient("Иван", "Иванов", "ivan@example.com");
    clientManager.addPhoneNumber(clientId, "123-456-7890");
    clientManager.addPhoneNumber(clientId, "987-654-3210");

    // Поиск клиента
    clientManager.findClient("Иван");

    // Обновление данных клиента
    clientManager.updateClient(clientId, "Петр", "Петров", "petr@example.com");

    // Поиск обновленного клиента
    clientManager.findClient("Петр");

    // Удаление клиента
    clientManager.removeClient(clientId);

    return 0;
}