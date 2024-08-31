#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <chrono>
#include <vector>
#include <string>
#include <cmath>
#include "classes.cpp"

using namespace std;

// Function to check if a file exists
bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

// Function to read data from CSV file
vector<vector<string>> readCSV(const string& filename) {
    vector<vector<string>> data;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        vector<string> row;
        string cell;
        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        data.push_back(row);
    }
    return data;
}

// Function to write data to CSV file
void writeCSV(const string& filename, const vector<vector<string>>& data) {
    ofstream file(filename);
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i != row.size() - 1) {
                file << ",";
            }
        }
        file << endl;
    }
}

// Function to remove a row from CSV file
void removeRow(const string& filename, int row) {
    vector<vector<string>> data = readCSV(filename);
    data.erase(data.begin() + row);
    writeCSV(filename, data);
}

// Function to append a row to CSV file
void appendRow(const string& filename, const vector<string>& newRow) {
    vector<vector<string>> data = readCSV(filename);
    data.push_back(newRow);
    writeCSV(filename, data);
}

// Funtion to modify a row in CSV file
void modifyRow(const string& filename, int row, const vector<string>& newRow) {
    vector<vector<string>> data = readCSV(filename);
    data[row] = newRow;
    writeCSV(filename, data);
}

void customer::show_available_cars()
{
    vector<vector<string>> data = readCSV(car::file);
    int flg = 0;
    for (auto i:data)
    {
        if (i[5] == "0")
        {
            flg = 1;
            std::cout<<"Car ID: "<<i[0]<<"\tCar Name: "<<i[1]<<"\tCar Model: "<<i[2]<<"\tCar Color: "<<i[3]<<"\tCar Condition: "<<i[4]<<endl;
        }
    }
    if (!flg) std::cout<<"No cars available"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
}

void customer::show_rented_cars()
{
    vector<vector<string>> data = readCSV(car::file);
    int flg = 0;
    for (auto i:data)
    {
        if (i[5] == "1" && i[8] == this->id)
        {   
            flg = 1;
            std::cout<<"Car ID: "<<i[0]<<"\tCar Name: "<<i[1]<<"\tCar Model: "<<i[2]<<"\tCar Color: "<<i[3]<<"\tCar Condition: "<<i[4]<<"\tDue Date: "<<i[6]<<endl;
        }
    }
    if (!flg) std::cout<<"No cars rented"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
}

string calculate_due_day(int days)
{
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);

    // Add the days to the current time
    std::chrono::system_clock::time_point now_tp = std::chrono::system_clock::from_time_t(t);
    now_tp += std::chrono::hours(24 * days);
    t = std::chrono::system_clock::to_time_t(now_tp);
    now = std::localtime(&t);

    // Create an output string stream
    std::ostringstream oss;

    // Write the future date into the string stream in the format dd-mm-yyyy
    oss << std::put_time(now, "%d-%m-%Y");

    // Convert the string stream into a string
    std::string future_date = oss.str();

    return future_date;
}

void customer::rent_car()
{
    if (this->fine > 0)
    {
        std::cout<<"You have a fine of "<<this->fine<<" rupees. You cannot rent a car until you pay the fine"<<endl;
        std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
        return;
    }

    vector<vector<string>> data = readCSV(car::file);
    if (((int)this->record) <= this->number_of_rented_cars)
    {
        std::cout<<"Your record is "<<this->record<<" and you have presently rented "<<this->number_of_rented_cars<<" cars.\nSo you cannot rent more cars."<<endl;
        std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
        return;
    }

    int cars_available = 0;
    for (auto i:data)
    {
        if (i[5] == "0")
        {
            cars_available++;
        }
    }
    if (cars_available == 0)
    {
        std::cout<<"No cars available"<<endl;
        std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
        return;
    }

    std::cout<<"Enter the ID of the car you want to rent: ";
    string car_id;
    cin>>car_id;
    std::cout<<endl;

    int flg = 0;
    for (int i = 0; i < data.size(); ++i)
    {
        if (data[i][0] == car_id)
        {
            if (data[i][5] == "0")
            {
                data[i][5] = "1";
                int days = -1;
                std::cout<<"Enter the number of days you want to rent the car: ";
                while (days <= 0)
                {
                    cin>>days;
                    if (days <= 0) std::cout<<"Invalid input.\nPlease enter a valid number of days: ";
                }
                std::cout<<"You need to pay "<<(int) (days * car::cost_per_day *(1 - (this->discount/100.0)))<<" rupees"<<endl;
                std::cout<<"Car rented successfully"<<endl;
                data[i][6] = calculate_due_day(days);
                data[i][8] = this->id;
                data[i][9] = this->name;
                modifyRow(car::file, i, data[i]);
                this->number_of_rented_cars++;
                
                vector<vector<string>> customer_data;
                if (this->id[0] == 'c') customer_data = readCSV(customer::customer_file);
                else customer_data = readCSV(customer::employee_file);

                for (int j = 0; j < customer_data.size(); ++j)
                {
                    if (customer_data[j][0] == this->id)
                    {
                        customer_data[j][6] = to_string(this->number_of_rented_cars);
                        if (this->id[0] == 'c') modifyRow(customer::customer_file, j, customer_data[j]);
                        else modifyRow(customer::employee_file, j, customer_data[j]);
                        break;
                    }
                }

                flg = 1;
                break;
            }
            else
            {
                std::cout<<"Car is already rented"<<endl;
                flg = 1;
                break;
            }
        }
    }
    if (!flg) std::cout<<"Car not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
}

int calculate_fine(string due_date_str)
{
    int day1, month1, year1;
    char ch;
    stringstream ss(due_date_str);
    ss >> day1 >> ch >> month1 >> ch >> year1;
    struct tm t1 = {0, 0, 0, day1, month1-1, year1-1900};
    time_t time1 = mktime(&t1);
    time_t now = time(0);
    double seconds = difftime(now, time1); // now - time1

    if (seconds <= 0) return 0;
    else return ( std::ceil(seconds / 86400.0) )* car::fine_per_day;
}

void customer::return_car()
{
    if (this->number_of_rented_cars == 0)
    {
        std::cout<<"You have not rented any cars"<<endl;
        std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
        return;
    }

    this->show_rented_cars();
    std::cout<<"You have the above rented cars\nEnter the ID of the car you want to return: ";
    string car_id;
    cin>>car_id;
    std::cout<<endl;

    vector<vector<string>> data = readCSV(car::file);
    int flg = 0;
    for (int i = 0; i < data.size(); ++i)
    {
        if (data[i][0] == car_id)
        {
            if (data[i][5] == "1" && data[i][8] == this->id)
            {
                double damage;
                std::cout<<"Is there any damage to the car?\nEnter the amount of damage [0 means no damage; 10 means heavy damage]: ";
                cin>>damage;
                damage = (int) std::min(10.0, std::max(0.0, damage));

                int fine = damage*car::damage_cost + calculate_fine(data[i][6]);

                if (fine > 0)
                {
                    std::cout<<"You are "<<calculate_fine(data[i][6])/car::fine_per_day<<" days late"<<endl;
                    std::cout<<"Your damage fine is "<<damage*car::damage_cost<<" rupees"<<endl;
                    std::cout<<"You need to pay a fine of "<<fine<<" rupees"<<endl;
                    std::cout<<"This fine has been added to you account"<<endl;
                    this->record--;

                    if (this->id[0] == 'c') customer::customer_avg_record = (customer::customer_avg_record * (customer::number_of_customers - 1) + this->record) / customer::number_of_customers;
                    else customer::employee_avg_record = (customer::employee_avg_record * (customer::number_of_employees - 1) + this->record) / customer::number_of_employees;
                }

                this->fine += fine;
                this->number_of_rented_cars--;
                vector<vector<string>> customer_data;
                if (this->id[0] == 'c') customer_data = readCSV(customer::customer_file);
                else customer_data = readCSV(customer::employee_file);

                for (int j = 0; j < customer_data.size(); ++j)
                {
                    if (customer_data[j][0] == this->id)
                    {  
                        customer_data[j][3] = to_string(this->fine);
                        customer_data[j][4] = to_string(this->record);
                        customer_data[j][6] = to_string(this->number_of_rented_cars);
                        if (this->id[0] == 'c') modifyRow(customer::customer_file, j, customer_data[j]);
                        else modifyRow(customer::employee_file, j, customer_data[j]);
                        break;
                    }
                }

                std::cout<<"Car returned successfully"<<endl;
                data[i][5] = "0";
                data[i][6] = "0";
                data[i][7] = to_string((int) damage);
                data[i][8] = "0";
                data[i][9] = "0";
                modifyRow(car::file, i, data[i]);
                flg = 1;
                break;
            }
            else
            {
                std::cout<<"Car is not rented by you"<<endl;
                flg = 1;
                break;
            }
        }
    }
    if (!flg) std::cout<<"Car not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
}

void customer::clear_fine()
{
    if (this->fine == 0)
    {
        std::cout<<"You do not have any fine"<<endl;
        std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
        return;
    }
    std::cout<<"You have a fine of "<<this->fine<<" rupees"<<endl;
    std::cout<<"Do you want to clear the fine? [y/n]: ";
    char choice;
    cin>>choice;
    if (choice == 'y')
    {
        this->fine = 0;
        vector<vector<string>> data;
        if (this->id[0] == 'c') data = readCSV(customer::customer_file);
        else data = readCSV(customer::employee_file);

        for (int i = 0; i < data.size(); ++i)
        {
            if (data[i][0] == this->id)
            {
                data[i][3] = "0";
                if (this->id[0] == 'c') modifyRow(customer::customer_file, i, data[i]);
                else modifyRow(customer::employee_file, i, data[i]);
                break;
            }
        }
        std::cout<<"Fine cleared successfully"<<endl;
    }
    else if (choice == 'n')
    {
        std::cout<<"Fine not cleared"<<endl;
    }
    else
    {
        std::cout<<"Invalid choice"<<endl;
    }
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void customer::view_profile()
{
    std::cout<<"Your ID: "<<this->id<<"\tYour Name: "<<this->name<<"\tYour Password: "<<this->password<<"\tYour Fine: "<<this->fine<<"\tYour Record: "<<this->record<<"\tYour Number of Rented Cars: "<<this->number_of_rented_cars<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
}

void manager::show_all_cars()
{
    vector<vector<string>> data = readCSV(car::file);
    int flag = 0;
    for (auto i:data)
    {
        flag = 1;
        std::cout<<"Car ID: "<<i[0]<<"\tCar Name: "<<i[1]<<"\tCar Model: "<<i[2]<<"\tCar Color: "<<i[3]<<"\tCar Condition: "<<i[4]<<"\tBooking Status: "<<i[5]<<"\tDue Date: "<<i[6]<<"\tDamage: "<<i[7]<<"\tRenter ID: "<<i[8]<<"\tRenter Name: "<<i[9]<<endl;
    }
    if (!flag) std::cout<<"No cars present"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::show_all_customers()
{
    vector<vector<string>> data = readCSV(customer::customer_file);
    int flag = 0;
    for (auto i:data)
    {
        flag = 1;
        std::cout<<"Customer ID: "<<i[0]<<"\tCustomer Name: "<<i[1]<<"\tCustomer Password: "<<i[2]<<"\tCustomer Fine: "<<i[3]<<"\tCustomer Record: "<<i[4]<<endl;
    }
    if (!flag) std::cout<<"No customers available"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::show_all_employees()
{
    vector<vector<string>> data = readCSV(customer::employee_file);
    int flag = 0;
    for (auto i:data)
    {
        flag = 1;
        std::cout<<"Employee ID: "<<i[0]<<"\tEmployee Name: "<<i[1]<<"\tEmployee Password: "<<i[2]<<"\tEmployee Fine: "<<i[3]<<"\tEmployee Record: "<<i[4]<<endl;
    }
    if (!flag) std::cout<<"No employees available"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::show_all_managers(vector<vector<string>> &managers)
{
    int flag = 0;
    for (auto i:managers)
    {
        flag = 1;
        std::cout<<"Manager ID: "<<i[0]<<"\tManager Name:"<<i[1]<<"\tManager Password: "<<i[2]<<endl;
    }
    if (!flag) std::cout<<"No managers available"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void car::add_car()
{
    appendRow(car::file, {this->id, this->name, this->model, this->color, this->condition, "0", "0", "0", "0", "0"});
    car::car_id_count++;
    std::cout<<"Car added successfully"<<endl;
    std::cout<<"The car ID is "<<id<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void car::remove_car()
{
    vector<vector<string>> data = readCSV(car::file);
    for (int i = 0; i < data.size(); ++i)
    {
        if (data[i][0] == this->id && data[i][5] == "0")
        {
            removeRow(car::file, i);
            std::cout<<"Car removed successfully"<<endl;
            break;
        }
        else if (data[i][0] == this->id && data[i][5] == "1")
        {
            std::cout<<"Car is presently rented. So it cannot be removed"<<endl;
            break;
        }
    }
    return;
}

void car::update_car()
{
    vector<vector<string>> data = readCSV(car::file);
    for (int i = 0; i < data.size(); ++i)
    {
        if (data[i][0] == this->id && data[i][5] == "0")
        {
            car curr_car(data[i][0], data[i][1], data[i][2], data[i][3], data[i][4], stoi(data[i][5]), stoi(data[i][6]), stoi(data[i][7]), data[i][8], data[i][9]);
            std::cout<<"Please enter the updated details of the car"<<endl;
            std::cout<<"Enter the color of the car: ";
            cin>>curr_car.color;
            std::cout<<"Enter the condition of the car: ";
            cin>>curr_car.condition;
            std::cout<<"Enter the booking status of the car [0 means not booked; 1 means booked]: ";
            cin>>curr_car.is_booked;
            std::cout<<"Enter the due date of the car: ";
            cin>>curr_car.due_date;
            std::cout<<"Enter the damage of the car: ";
            cin>>curr_car.damage;
            std::cout<<"Enter the renter ID of the car: ";  
            cin>>curr_car.renter_id;
            std::cout<<"Enter the renter name of the car: ";
            cin>>curr_car.renter_name;

            data[i][3] = curr_car.color;
            data[i][4] = curr_car.condition;
            data[i][5] = to_string(curr_car.is_booked);
            data[i][6] = to_string(curr_car.due_date);
            data[i][7] = to_string(curr_car.damage);
            data[i][8] = curr_car.renter_id;
            data[i][9] = curr_car.renter_name;
            modifyRow(car::file, i, data[i]);
            std::cout<<"Car updated successfully"<<endl;
            break;
        }
        else if (data[i][0] == this->id && data[i][5] == "1")
        {
            std::cout<<"Car is presently rented. So it cannot be updated"<<endl;
            break;
        }
    }
    return;
}

void manager::add_customer()
{
    std::cout<<"Enter the name of the customer: ";
    string name;
    cin>>name;
    std::cout<<"Enter the password of the customer: ";
    string password;
    cin>>password;
    std::cout<<endl;

    string id = "c";
    id += to_string(customer::customer_id_count);
    appendRow(customer::customer_file, {id, name, password, "0", to_string(customer::customer_avg_record), "0", "0"});
    std::cout<<"Customer added successfully"<<endl;
    std::cout<<"The customer ID is "<<id<<endl;
    customer::number_of_customers++;
    customer::customer_id_count++;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::remove_customer()
{
    std::cout<<"Enter the ID of the customer you want to remove: ";
    string customer_id;
    cin>>customer_id;
    std::cout<<endl;

    vector<vector<string>> data = readCSV(customer::customer_file);
    int flg = 0;
    for (int i = 0; i < data.size(); ++i)
    {
        if (data[i][0] == customer_id)
        {
            int count_booked_cars = stoi(data[i][6]);
            if (count_booked_cars == 0)
            {
                removeRow(customer::customer_file, i);
                flg = 1;
                std::cout<<"Customer removed successfully"<<endl;
                break;
            }
            else
            {
                std::cout<<"Customer has rented cars. Do you still want to remove him? [y/n]:";
                char choice;
                cin>>choice;
                if (choice == 'y')
                {
                    removeRow(customer::customer_file, i);
                    std::cout<<"Customer removed successfully"<<endl;
                    customer::number_of_customers--;
                }
                else
                {
                    std::cout<<"Customer not removed"<<endl;
                }
                flg = 1;
                break;
            }
        }
    }
    if (!flg) std::cout<<"Customer not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::update_customer()
{
    std::cout<<"Enter the ID of the customer you want to update: ";
    string customer_id;
    cin>>customer_id;
    std::cout<<endl;

    vector<vector<string>> data = readCSV(customer::customer_file);
    int flg = 0;
    for (int i = 0; i < data.size(); i++)
    {
        if (data[i][0] == customer_id)
        {
            std::cout<<"Please enter the updated details of the customer"<<endl;
            std::cout<<"Enter the new name of the customer: ";
            cin>>data[i][1];
            std::cout<<"Enter the new password of the customer: ";
            cin>>data[i][2];
            std::cout<<"Enter the updated fine of the customer: ";
            cin>>data[i][3];
            std::cout<<"Enter the updated record of the customer: ";
            cin>>data[i][4];
            std::cout<<"Enter the updated number of cars rented by the customer: ";
            cin>>data[i][6];

            modifyRow(customer::customer_file, i, data[i]);
            flg = 1;
            std::cout<<"Customer updated successfully"<<endl;
            break;
        }
    }
    if (!flg) std::cout<<"Customer not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::add_employee()
{
    std::cout<<"Enter the name of the employee: ";
    string name;
    cin>>name;
    std::cout<<"Enter the password of the employee: ";
    string password;
    cin>>password;
    std::cout<<endl;

    string id = "e";
    id += to_string(customer::employee_id_count);
    appendRow(customer::employee_file, {id, name, password, "0", to_string(customer::employee_avg_record), "15", "0"});
    std::cout<<"Employee added successfully"<<endl;
    std::cout<<"The employee ID is "<<id<<endl;
    customer::number_of_employees++;
    customer::employee_id_count++;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::remove_employee()
{
    std::cout<<"Enter the id of the employee you want to remove: ";
    string employee_id;
    cin>>employee_id;
    std::cout<<endl;

    vector<vector<string>> data = readCSV(customer::employee_file);
    int flg = 0;
    for (int i = 0; i < data.size(); ++i)
    {
        if (data[i][0] == employee_id)
        {
            int booked_cars = stoi(data[i][6]);
            if (booked_cars == 0)
            {
                removeRow(customer::employee_file, i);
                flg = 1;
                std::cout<<"Employee removed successfully"<<endl;
                break;
            }
            else
            {
                std::cout<<"Employee has rented cars. Do you still want to remove him? [y/n]:";
                char choice;
                cin>>choice;
                if (choice == 'y')
                {
                    removeRow(customer::employee_file, i);
                    std::cout<<"Employee removed successfully"<<endl;
                    customer::number_of_employees--;
                }
                else
                {
                    std::cout<<"Employee not removed"<<endl;
                }
                flg = 1;
                break;
            }
        }
    }
    if (!flg) std::cout<<"Employee not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::update_employee()
{
    std::cout<<"Enter the ID of the employee you want to update: ";
    string employee_id;
    cin>>employee_id;
    std::cout<<endl;

    vector<vector<string>> data = readCSV(customer::employee_file);
    int flg = 0;
    for (int i = 0; i < data.size(); ++i)
    {
        if (data[i][0] == employee_id)
        {
            std::cout<<"Please enter the updated details of the employee"<<endl;
            std::cout<<"Enter the new name of the employee: ";
            cin>>data[i][1];
            std::cout<<"Enter the new password of the employee: ";
            cin>>data[i][2];
            std::cout<<"Enter the updated fine of the employee: ";
            cin>>data[i][3];
            std::cout<<"Enter the updated record of the employee: ";
            cin>>data[i][4];
            std::cout<<"Enter the updated number of cars rented by the employee: ";
            cin>>data[i][6];

            modifyRow(customer::customer_file, i, data[i]);
            flg = 1;
            std::cout<<"Employee updated successfully"<<endl;
            break;
        }
    }
    if (!flg) std::cout<<"Employee not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::add_manager(vector<vector<string>> &managers)
{
    std::cout<<"Enter the name of the manager: ";
    string name;
    cin>>name;
    std::cout<<"Enter the password of the manager: ";
    string password;
    cin>>password;
    std::cout<<endl;

    string id = "m";
    id += to_string(manager::manager_id_count);
    managers.push_back({id, name, password});
    std::cout<<"Manager added successfully with manager ID "<<id<<endl;
    manager::manager_id_count++;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::remove_manager(vector<vector<string>> &managers)
{
    std::cout<<"Enter the id of the manager you want to remove: ";
    string manager_id;
    cin>>manager_id;
    std::cout<<endl;

    if (this->id == manager_id)
    {
        std::cout<<"You cannot remove yourself"<<endl;
        std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
        return;
    }

    int flg = 0;
    for (int i = 0; i < managers.size(); ++i)
    {
        if (managers[i][0] == manager_id)
        {
            managers.erase(managers.begin() + i);
            flg = 1;
            std::cout<<"Manager removed successfully"<<endl;
            break;
        }
    }
    if (!flg) std::cout<<"Manager not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::update_manager(vector<vector<string>> &managers)
{
    std::cout<<"Enter the ID of the manager you want to update: ";
    string manager_id;
    cin>>manager_id;
    std::cout<<endl;

    int flg = 0;
    for (int i = 0; i < managers.size(); ++i)
    {
        if (managers[i][0] == manager_id)
        {
            std::cout<<"Please enter the updated details of the manager"<<endl;
            std::cout<<"Enter the name of the manager: ";
            string name;
            cin>>name;
            std::cout<<"Enter the password of the manager: ";
            string password;
            cin>>password;
            std::cout<<endl;

            managers[i][1] = name;
            managers[i][2] = password;
            flg = 1;
            std::cout<<"Manager updated successfully"<<endl;
            break;
        }
    }
    if (!flg) std::cout<<"Manager not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::search_customer_by_id()
{
    std::cout<<"Enter the ID of the customer you want to search: ";
    string customer_id;
    cin>>customer_id;
    std::cout<<endl;

    vector<vector<string>> data = readCSV(customer::customer_file);
    int flg = 0;
    for (auto i:data)
    {
        if (i[0] == customer_id)
        {
            flg = 1;
            std::cout<<"Customer ID: "<<i[0]<<"\tCustomer Name: "<<i[1]<<"\tCustomer Password: "<<i[2]<<"\tCustomer Fine: "<<i[3]<<"\tCustomer Record: "<<i[4]<<"\tNumber of rented cars: "<<i[6]<<endl;
            break;
        }
    }
    if (!flg) std::cout<<"Customer not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::search_customer_by_name()
{
    std::cout<<"Enter the name of the customer you want to search: ";
    string customer_name;
    cin>>customer_name;
    std::cout<<endl;

    vector<vector<string>> data = readCSV(customer::customer_file);
    int flg = 0;
    for (auto i:data)
    {
        if (i[1] == customer_name)
        {
            flg = 1;
            std::cout<<"Customer ID: "<<i[0]<<"\tCustomer Name: "<<i[1]<<"\tCustomer Password: "<<i[2]<<"\tCustomer Fine: "<<i[3]<<"\tCustomer Record: "<<i[4]<<"\tNumber of cars rented: "<<i[6]<<endl;
        }
    }
    if (!flg) std::cout<<"Customer not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::search_employee_by_id()
{
    std::cout<<"Enter the ID of the employee you want to search: ";
    string employee_id;
    cin>>employee_id;
    std::cout<<endl;

    vector<vector<string>> data = readCSV(customer::employee_file);
    int flg = 0;
    for (auto i:data)
    {
        if (i[0] == employee_id)
        {
            flg = 1;
            std::cout<<"Employee ID: "<<i[0]<<"\tEmployee Name: "<<i[1]<<"\tEmployee Password: "<<i[2]<<"\tEmployee Fine: "<<i[3]<<"\tEmployee Record: "<<i[4]<<"\tNumber of cars rented: "<<i[6]<<endl;
            break;
        }
    }
    if (!flg) std::cout<<"Employee not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::search_employee_by_name()
{
    std::cout<<"Enter the name of the employee you want to search: ";
    string employee_name;
    cin>>employee_name;
    std::cout<<endl;

    vector<vector<string>> data = readCSV(customer::employee_file);
    int flg = 0;
    for (auto i:data)
    {
        if (i[1] == employee_name)
        {
            flg = 1;
            std::cout<<"Employee ID: "<<i[0]<<"\tEmployee Name: "<<i[1]<<"\tEmployee Password: "<<i[2]<<"\tEmployee Fine: "<<i[3]<<"\tEmployee Record: "<<i[4]<<"\tNumber of cars rented: "<<i[6]<<endl;
        }
    }
    if (!flg) std::cout<<"Employee not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::search_manager_by_id(vector<vector<string>> &managers)
{
    std::cout<<"Enter the ID of the manager you want to search: ";
    string manager_id;
    cin>>manager_id;
    std::cout<<endl;

    int flg = 0;
    for (auto i:managers)
    {
        if (i[0] == manager_id)
        {
            flg = 1;
            std::cout<<"Manager ID: "<<i[0]<<"\tManager Name: "<<i[1]<<"\tManager Password: "<<i[2]<<endl;
            break;
        }
    }
    if (!flg) std::cout<<"Manager not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::search_manager_by_name(vector<vector<string>> &managers)
{
    std::cout<<"Enter the name of the manager you want to search: ";
    string manager_name;
    cin>>manager_name;
    std::cout<<endl;

    int flg = 0;
    for (auto i:managers)
    {
        if (i[1] == manager_name)
        {
            flg = 1;
            std::cout<<"Manager ID: "<<i[0]<<"\tManager Name: "<<i[1]<<"\tManager Password: "<<i[2]<<endl;
        }
    }
    if (!flg) std::cout<<"Manager not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::search_car_by_id()
{
    std::cout<<"Enter the ID of the car you want to search: ";
    string car_id;
    cin>>car_id;
    std::cout<<endl;

    vector<vector<string>> data = readCSV(car::file);
    int flg = 0;
    for (auto i:data)
    {
        if (i[0] == car_id)
        {
            flg = 1;
            std::cout<<"Car ID: "<<i[0]<<"\tCar Name: "<<i[1]<<"\tCar Model: "<<i[2]<<"\tCar Color: "<<i[3]<<"\tCar Condition: "<<i[4]<<"\tBooking Status: "<<i[5]<<"\tDue Date: "<<i[6]<<"\tDamage: "<<i[7]<<"\tRenter ID: "<<i[8]<<"\tRenter Name: "<<i[9]<<endl;
            break;
        }
    }
    if (!flg) std::cout<<"Car not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::search_car_by_name()
{
    std::cout<<"Enter the name of the car you want to search: ";
    string car_name;
    cin>>car_name;
    std::cout<<endl;

    vector<vector<string>> data = readCSV(car::file);
    int flg = 0;
    for (auto i:data)
    {
        if (i[1] == car_name)
        {
            flg = 1;
            std::cout<<"Car ID: "<<i[0]<<"\tCar Name: "<<i[1]<<"\tCar Model: "<<i[2]<<"\tCar Color: "<<i[3]<<"\tCar Condition: "<<i[4]<<"\tBooking Status: "<<i[5]<<"\tDue Date: "<<i[6]<<"\tDamage: "<<i[7]<<"\tRenter ID: "<<i[8]<<"\tRenter Name: "<<i[9]<<endl;
        }
    }
    if (!flg) std::cout<<"Car not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::search_car_by_model()
{
    std::cout<<"Enter the model of the car you want to search: ";
    string car_model;
    cin>>car_model;
    std::cout<<endl;

    vector<vector<string>> data = readCSV(car::file);
    int flg = 0;
    for (auto i:data)
    {
        if (i[2] == car_model)
        {
            flg = 1;
            std::cout<<"Car ID: "<<i[0]<<"\tCar Name: "<<i[1]<<"\tCar Model: "<<i[2]<<"\tCar Color: "<<i[3]<<"\tCar Condition: "<<i[4]<<"\tBooking Status: "<<i[5]<<"\tDue Date: "<<i[6]<<"\tDamage: "<<i[7]<<"\tRenter ID: "<<i[8]<<"\tRenter Name: "<<i[9]<<endl;
        }
    }
    if (!flg) std::cout<<"Car not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::search_car_by_color()
{
    std::cout<<"Enter the color of the car you want to search: ";
    string car_color;
    cin>>car_color;
    std::cout<<endl;

    vector<vector<string>> data = readCSV(car::file);
    int flg = 0;
    for (auto i:data)
    {
        if (i[3] == car_color)
        {
            flg = 1;
            std::cout<<"Car ID: "<<i[0]<<"\tCar Name: "<<i[1]<<"\tCar Model: "<<i[2]<<"\tCar Color: "<<i[3]<<"\tCar Condition: "<<i[4]<<"\tBooking Status: "<<i[5]<<"\tDue Date: "<<i[6]<<"\tDamage: "<<i[7]<<"\tRenter ID: "<<i[8]<<"\tRenter Name: "<<i[9]<<endl;
        }
    }
    if (!flg) std::cout<<"Car not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::search_car_by_condition()
{
    std::cout<<"Enter the condition of the car you want to search: ";
    string car_condition;
    cin>>car_condition;
    std::cout<<endl;

    vector<vector<string>> data = readCSV(car::file);
    int flg = 0;
    for (auto i:data)
    {
        if (i[4] == car_condition)
        {
            flg = 1;
            std::cout<<"Car ID: "<<i[0]<<"\tCar Name: "<<i[1]<<"\tCar Model: "<<i[2]<<"\tCar Color: "<<i[3]<<"\tCar Condition: "<<i[4]<<"\tBooking Status: "<<i[5]<<"\tDue Date: "<<i[6]<<"\tDamage: "<<i[7]<<"\tRenter ID: "<<i[8]<<"\tRenter Name: "<<i[9]<<endl;
        }
    }
    if (!flg) std::cout<<"Car not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::search_car_by_renter_ID()
{
    std::cout<<"Enter the renter ID of the car you want to search: ";
    string renter_id;
    cin>>renter_id;
    std::cout<<endl;

    vector<vector<string>> data = readCSV(car::file);
    int flg = 0;
    for (auto i:data)
    {
        if (i[8] == renter_id)
        {
            flg = 1;
            std::cout<<"Car ID: "<<i[0]<<"\tCar Name: "<<i[1]<<"\tCar Model: "<<i[2]<<"\tCar Color: "<<i[3]<<"\tCar Condition: "<<i[4]<<"\tBooking Status: "<<i[5]<<"\tDue Date: "<<i[6]<<"\tDamage: "<<i[7]<<"\tRenter ID: "<<i[8]<<"\tRenter Name: "<<i[9]<<endl;
        }
    }
    if (!flg) std::cout<<"Car not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::show_rented_cars()
{
    vector<vector<string>> data = readCSV(car::file);
    int flag = 0;
    for (auto i:data)
    {
        if (i[5] == "1")
        {
            flag = 1;
            std::cout<<"Car ID: "<<i[0]<<"\tCar Name: "<<i[1]<<"\tCar Model: "<<i[2]<<"\tCar Color: "<<i[3]<<"\tCar Condition: "<<i[4]<<"\tBooking Status: "<<i[5]<<"\tDue Date: "<<i[6]<<"\tDamage: "<<i[7]<<"\tRenter ID: "<<i[8]<<"\tRenter Name: "<<i[9]<<endl;
        }
    }
    if (!flag) std::cout<<"No cars are rented"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void manager::search_car_by_renter_name()
{
    std::cout<<"Enter the renter name of the car you want to search: ";
    string renter_name;
    cin>>renter_name;
    std::cout<<endl;

    vector<vector<string>> data = readCSV(car::file);
    int flg = 0;
    for (auto i:data)
    {
        if (i[9] == renter_name)
        {
            flg = 1;
            std::cout<<"Car ID: "<<i[0]<<"\tCar Name: "<<i[1]<<"\tCar Model: "<<i[2]<<"\tCar Color: "<<i[3]<<"\tCar Condition: "<<i[4]<<"\tBooking Status: "<<i[5]<<"\tDue Date: "<<i[6]<<"\tDamage: "<<i[7]<<"\tRenter ID: "<<i[8]<<"\tRenter Name: "<<i[9]<<endl;
        }
    }
    if (!flg) std::cout<<"Car not found"<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void login_as_customer()
{
    string userID, password;
    std::cout<<"Enter your UserID: ";
    cin>>userID;
    std::cout<<"Enter your Password: ";
    cin>>password;
    std::cout<<endl;

    int flag = 0, record_number = -1, login_customer_finish = 0;

    vector<vector<string>> data = readCSV(customer::customer_file);
    for (int i = 0; i < data.size(); ++i) {
        if (data[i][0] == userID) {
            if (data[i][2] == password) {
                record_number = i;
                break;
            }
            else{
                std::cout<<"Invalid Password\n";
                flag = 1;
                break;
            }
        }
    }
    if (flag) {
        std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
        return;
    }

    if (record_number == -1) {
        std::cout<<"UserID not found"<<endl;
        std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
        return;
    }

    std::cout<<"Hello "<<data[record_number][1]<<"!\n";
    customer curr_customer(data[record_number][0], data[record_number][1], data[record_number][2], stoi(data[record_number][3]), stoi(data[record_number][4]), stoi(data[record_number][5]), stoi(data[record_number][6]));

    while (!login_customer_finish)
    {
        std::cout<<"What do you want to do?\n1. Show available cars\n2. Show rented cars\n3. Rent a car\n4. Return a car\n5. Show fine\n6. Clear fine\n7. View profile\n8. Logout\n";
        std::cout<<"Enter your choice: ";
        int login_choice;
        cin>>login_choice;
        std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;

        switch (login_choice)
        {
        case 1:
            curr_customer.show_available_cars();        
            break;

        case 2:
            curr_customer.show_rented_cars();
            break;

        case 3:
            curr_customer.rent_car();
            break;

        case 4:
            curr_customer.return_car();
            break;

        case 5:
            std::cout<<"Your fine is "<<curr_customer.fine<<endl;
            std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
            break;
        
        case 6:
            curr_customer.clear_fine();
            break;

        case 7:
            curr_customer.view_profile();
            break;

        case 8:
            std::cout<<"You are successfully logged out"<<endl;
            std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
            login_customer_finish = 1;
            break;

        default:
            std::cout<<"Invalid choice"<<endl<<endl;
            break;
        }
    }
}

void login_as_employee()
{
    string userID, password;
    std::cout<<"Enter your UserID: ";
    cin>>userID;
    std::cout<<"Enter your Password: ";
    cin>>password;
    std::cout<<endl;

    int flag = 0, record_number = -1, login_employee_finish = 0;

    vector<vector<string>> data = readCSV(customer::employee_file);
    for (int i = 0; i < data.size(); ++i) {
        if (data[i][0] == userID) {
            if (data[i][2] == password) {
                record_number = i;
                break;
            }
            else{
                std::cout<<"Invalid Password"<<endl;
                flag = 1;
                break;
            }
        }
    }
    if (flag) {
        std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
        return;
    }

    if (record_number == -1) {
        std::cout<<"UserID not found"<<endl;
        std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
        return;
    }

    std::cout<<"Hello "<<data[record_number][1]<<"!\n";
    customer curr_employee(data[record_number][0], data[record_number][1], data[record_number][2], stoi(data[record_number][3]), stoi(data[record_number][4]), stoi(data[record_number][5]), stoi(data[record_number][6]));
    while (!login_employee_finish)
    {
        std::cout<<"What do you want to do?\n1. Show available cars\n2. Show rented cars\n3. Rent a car\n4. Return a car\n5. Show fine\n6. Clear fine\n7. View profile\n8. Logout\n";
        std::cout<<"Enter your choice: ";
        int login_choice;
        cin>>login_choice;
        std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;

        switch (login_choice)
        {
        case 1:
            curr_employee.show_available_cars();
            break;

        case 2:
            curr_employee.show_rented_cars();
            break;

        case 3:
            curr_employee.rent_car();
            break;

        case 4:
            curr_employee.return_car();
            break;

        case 5:
            std::cout<<"Your fine is "<<curr_employee.fine<<endl;
            std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
            break;

        case 6:
            curr_employee.clear_fine();
            break;

        case 7:
            curr_employee.view_profile();
            break;

        case 8:
            std::cout<<"You are successfully logged out"<<endl;
            std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
            login_employee_finish = 1;
            break;

        default:
            std::cout<<"Invalid choice"<<endl<<endl;
            break;
        }
    }
    return;
}

void login_as_manager(vector<vector<string>> &managers)
{
    string userID, password;
    std::cout<<"Enter your UserID: ";
    cin>>userID;
    std::cout<<"Enter your Password: ";
    cin>>password;
    std::cout<<endl;

    int flag = 0, record_number = -1, login_manager_finish = 0;

    for (int i = 0; i < managers.size(); i++)
    {
        if (managers[i][0]==userID)
        {
            if (managers[i][2]==password)
            {
                record_number = i;
                break;
            }
            else
            {
                std::cout<<"Invalid Password"<<endl;
                flag = 1;
                break;
            }
        }
    }
    
    if (flag) {
        std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
        return;
    }

    if (record_number == -1) {
        std::cout<<"UserID not found"<<endl<<endl;
        std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
        return;
    }

    std::cout<<"Hello "<<managers[record_number][1]<<"!\n";
    manager curr_manager(managers[record_number][0], managers[record_number][1], managers[record_number][2]);
    while (!login_manager_finish)
    {
        std::cout<<"What do you want to do?\n1. Show all cars\n2. Show all customers\n3. Show all employees\n4. Show all managers\n5. Add a car\n6. Remove a car\n7. Update a car\n8. Add a customer\n9. Remove a customer\n10. Update a customer\n11. Add an employee\n12. Remove an employee\n13. Update an employee\n14. Add a manager\n15. Remove a manager\n16. Update a manager\n17. Search for a customer\n18. Search for an employee\n19. Search for a manager\n20. Search for a car\n21. Show rented cars\n22. Logout\n";
        std::cout<<"Enter your choice: ";
        int login_choice;
        cin>>login_choice;
        std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
        int search_choice, go_back = 0, flg;
        string name, model, color, condition, car_id;
        vector<vector<string>> data;
        car curr_car("", "", "", "", "", 0, 0, 0, "", "");

        switch (login_choice)
        {
        case 1:
            curr_manager.show_all_cars();
            break;
        case 2:
            curr_manager.show_all_customers();
            break;
        case 3:
            curr_manager.show_all_employees();
            break;
        case 4:
            curr_manager.show_all_managers(managers);
            break;
        case 5:
            std::cout<<"Enter the name of the car: ";
            cin>>name;
            std::cout<<"Enter the model of the car: ";
            cin>>model;
            std::cout<<"Enter the color of the car: ";
            cin>>color;
            std::cout<<"Enter the condition of the car: ";
            cin>>condition;
            std::cout<<endl;

            car_id = "car";
            car_id += to_string(car::car_id_count);
            curr_car.set_val(car_id, name, model, color, condition, 0, 0, 0, "0", "0");
            curr_car.add_car();
            break;
        case 6:
            std::cout<<"Enter the ID of the car you want to remove: ";
            cin>>car_id;
            std::cout<<endl;

            data = readCSV(car::file);
            flg = 0;
            for (int i = 0; i < data.size(); ++i)
            {
                if (data[i][0] == car_id)
                {
                    curr_car.set_val(data[i][0], data[i][1], data[i][2], data[i][3], data[i][4], stoi(data[i][5]), stoi(data[i][6]), stoi(data[i][7]), data[i][8], data[i][9]);
                    curr_car.remove_car();
                    flg = 1;
                    break;
                }
            }
            if (!flg) std::cout<<"Car not found"<<endl;
            std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
            break;
        case 7:
            std::cout<<"Enter the ID of the car you want to update: ";
            cin>>car_id;
            std::cout<<endl;

            data = readCSV(car::file);
            flg = 0;
            for (int i = 0; i < data.size(); ++i)
            {
                if (data[i][0] == car_id)
                {
                    curr_car.set_val(data[i][0], data[i][1], data[i][2], data[i][3], data[i][4], stoi(data[i][5]), stoi(data[i][6]), stoi(data[i][7]), data[i][8], data[i][9]);
                    curr_car.update_car();
                    flg = 1;
                    break;
                }
            }
            if (!flg) std::cout<<"Car not found"<<endl;
            std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
            break;
        case 8:
            curr_manager.add_customer();
            break;
        case 9:
            curr_manager.remove_customer();
            break; 
        case 10:
            curr_manager.update_customer();
            break;
        case 11:
            curr_manager.add_employee();
            break;
        case 12:
            curr_manager.remove_employee();
            break;
        case 13:
            curr_manager.update_employee();
            break;
        case 14:
            curr_manager.add_manager(managers);
            break;
        case 15:
            curr_manager.remove_manager(managers);
            break;
        case 16:
            curr_manager.update_manager(managers);
            break;
        case 17:            
            go_back = 0;
            while (!go_back)
            {
                std::cout<<"1. Search by customer ID\n2. Search by customer name\n3. Go Back\n";
                std::cout<<"Enter your choice: ";
                cin>>search_choice;
                switch (search_choice)
                {
                case 1:
                    curr_manager.search_customer_by_id();
                    break;
                case 2:
                    curr_manager.search_customer_by_name();
                    break;
                case 3:
                    go_back = 1;
                    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
                    break;
                default:
                    std::cout<<"Invalid choice. Please enter a valid choice"<<endl;
                    break;
                }
            }            
            break;
        case 18:
            go_back = 0;
            while (!go_back)
            {
                std::cout<<"1. Search by employee ID\n2. Search by employee name\n3. Go Back\n";
                std::cout<<"Enter your choice: ";
                cin>>search_choice;
                switch (search_choice)
                {
                case 1:
                    curr_manager.search_employee_by_id();
                    break;
                case 2:
                    curr_manager.search_employee_by_name();
                    break;
                case 3:
                    go_back = 1;
                    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
                    break;
                default:
                    std::cout<<"Invalid choice. Please enter a valid choice"<<endl;
                    break;
                }
            }
            break;
        case 19:
            go_back = 0;
            while (!go_back)
            {
                std::cout<<"1. Search by manager ID\n2. Search by manager name\n3. Go Back\n";
                std::cout<<"Enter your choice: ";
                cin>>search_choice;
                switch (search_choice)
                {
                case 1:
                    curr_manager.search_manager_by_id(managers);
                    break;
                case 2:
                    curr_manager.search_manager_by_name(managers); 
                    break;
                case 3:
                    go_back = 1;
                    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
                    break;
                default:
                    std::cout<<"Invalid choice. Please enter a valid choice"<<endl;
                    break;
                }  
            }
            break;
        case 20:
            go_back = 0;
            while (!go_back)
            {
                std::cout<<"1. Search by car ID\n2. Search by car name\n3. Search by car model\n4. Search by car color\n5. Search by car condition\n6. Search by renter ID\n7. Search by renter name\n8. Go Back\n";
                std::cout<<"Enter your choice: ";
                cin>>search_choice;
                switch (search_choice)
                {
                case 1:
                    curr_manager.search_car_by_id();
                    break;
                case 2:
                    curr_manager.search_car_by_name();
                    break;
                case 3:
                    curr_manager.search_car_by_model();
                    break;
                case 4:
                    curr_manager.search_car_by_color();
                    break;
                case 5:
                    curr_manager.search_car_by_condition();
                    break;
                case 6:
                    curr_manager.search_car_by_renter_ID();
                    break;
                case 7:
                    curr_manager.search_car_by_renter_name();
                    break;
                case 8:
                    go_back = 1;
                    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
                    break;
                default:
                    std::cout<<"Invalid choice. Please enter a valid choice"<<endl;
                    break;
                }
            }
            break;
        case 21:
            curr_manager.show_rented_cars();
            break;
        case 22:
            std::cout<<"You are successfully logged out"<<endl;
            std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
            login_manager_finish = 1;
            break;

        default:
            std::cout<<"Invalid choice. Please enter a valid choice"<<endl<<endl;
            break;
        }
    }
    return;
}

void signup_customer()
{
    string username, password;
    std::cout<<"Enter your Username: ";
    cin>>username;
    std::cout<<"Enter your Password: ";
    cin>>password;

    vector<vector<string>> data = readCSV(customer::customer_file);    
    string id = "c";
    id += to_string(customer::customer_id_count);
    appendRow(customer::customer_file, {id, username, password, "0", to_string(customer::customer_avg_record), "0", "0"});
    customer::number_of_customers++;
    customer::customer_id_count++;
    std::cout<<"Congratulations! You have successfully signed up as a customer\nYour UserID is "<<id<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

void signup_employee()
{
    string username, password;
    std::cout<<"Enter your Username: ";
    cin>>username;
    std::cout<<"Enter your Password: ";
    cin>>password;

    vector<vector<string>> data = readCSV(customer::employee_file);
    string id = "e";
    id += to_string(customer::employee_id_count);
    appendRow(customer::employee_file, {id, username, password, "0", to_string(customer::employee_avg_record), "15", "0"});
    customer::number_of_employees++;
    customer::employee_id_count++;
    std::cout<<"Congratulations! You have successfully signed up as an employee\nYour UserID is "<<id<<endl;
    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
    return;
}

string customer::customer_file = "customer.csv"; // id, name, password, fine, record, discount, number_of_cars_rented
string customer::employee_file = "employee.csv"; // id, name, password, fine, record, discount, number_of_cars_rented
string car::file = "car.csv"; // id, name, model, color, condition, is_booked, due_date, damage, renter_id, renter_name

int customer::customer_avg_record = 5;
int customer::number_of_customers = 0;
int customer::customer_id_count = 0;

int customer::employee_avg_record = 5;
int customer::number_of_employees = 0;
int customer::employee_id_count = 0;

int manager::manager_id_count = 0;
int car::car_id_count = 0;
int car::cost_per_day = 1000;
int car::fine_per_day = 1200;
int car::damage_cost = 200;

int main()
{
    if (!fileExists(customer::customer_file)) {
        ofstream file(customer::customer_file);
    }
    if (!fileExists(customer::employee_file)) {
        ofstream file(customer::employee_file);
    }
    if (!fileExists(car::file)) {
        ofstream file(car::file);
    }

    try
    {
        vector<vector<string>> init_data = readCSV(customer::customer_file);

        if (init_data.size()!=0) customer::customer_avg_record = 0;
        for (int i = 0; i < init_data.size(); i++) {
            string id = init_data[i][0];
            for (int j=0;j<id.length()-1;j++) id[j] = id[j+1];
            id.pop_back();
            customer::customer_id_count = max(customer::customer_id_count, stoi(id));
            customer::customer_avg_record += stoi(init_data[i][4]);
            customer::number_of_customers++;
        }
        
        if (customer::number_of_customers != 0)
        {
            customer::customer_id_count++;
            customer::customer_avg_record /= customer::number_of_customers;
        }

        init_data = readCSV(customer::employee_file);
        if (init_data.size()!=0) customer::employee_avg_record = 0;
        for (int i = 0; i < init_data.size(); i++) {
            string id = init_data[i][0];
            for (int j=0;j<id.length()-1;j++) id[j] = id[j+1];
            id.pop_back();
            customer::employee_id_count = max(customer::employee_id_count, stoi(id));
            customer::employee_avg_record += stoi(init_data[i][4]);
            customer::number_of_employees++;
        }
        if (customer::number_of_employees != 0)
        {
            customer::employee_id_count++;
            customer::employee_avg_record /= customer::number_of_employees;
        }

        init_data = readCSV(car::file);
        for (int i = 0; i < init_data.size(); i++) {
            string id = init_data[i][0];
            for (int j=0;j<id.length()-3;j++) id[j] = id[j+3];
            id.pop_back();
            car::car_id_count = max(car::car_id_count, stoi(id));
        }
        if (init_data.size()!=0) car::car_id_count++;
    }
    catch(const std::exception& e)
    {
        // std::cerr 
    }
    
    vector<vector<string>> managers;
    managers.push_back({"m0","manager", "123"});
    manager::manager_id_count++;

    std::cout<<endl<<"-------------------------- Welcome to the Car Rental System --------------------------"<<endl<<endl;
    // std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;

    int finish = 0;
    int sign_up_finish = 0;
    int login_finish = 0;
    int login_customer_finish = 0;

    while (!finish)
    {
        std::cout<<"1. Login\n2. Sign up\n3. Exit\nPlease enter your choice: ";
        int choice;
        cin>>choice;
        std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
        
        switch (choice)
        {
        case 1: // Login
            login_finish = 0;
            while (!login_finish)
            {
                std::cout<<"1. Login as a customer\n2. Login as an employee\n3. Login as a manager\n4. Go back\n";
                std::cout<<"Please enter your choice: ";
                int subchoice;
                cin>>subchoice;
                std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;

                switch (subchoice)
                {
                case 1: // Login as a customer
                    login_as_customer();
                    break;

                case 2:
                    login_as_employee();
                    break;

                case 3:
                    login_as_manager(managers);
                    break;

                case 4:
                    login_finish = 1;
                    break;
                
                default:
                    std::cout<<"Invalid choice. Please enter a valid choice\n"<<endl;
                    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
                    break;
                }
            }
            break;

        case 2: // Sign up
            sign_up_finish = 0;
            while (!sign_up_finish)
            {
                std::cout<<"1. Signup as a customer\n2. Signup as an employee\n3. Go back\n";
                std::cout<<"Please enter your choice: ";
                int subchoice;
                cin>>subchoice;
                std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
                
                switch (subchoice)
                {
                case 1: // Signup as a customer
                    signup_customer();
                    break;

                case 2:  // Signup as an employee
                    signup_employee();
                    break;

                case 3:
                    sign_up_finish = 1;
                    break;
                
                default:
                    std::cout<<"Invalid choice. Please enter a valid choice\n";
                    std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
                    break;
                }
            }

            break;

        case 3:
            finish = 1;
            std::cout<<"Thank you for using the Car Rental System\n\n";
            break;
        
        default:
            std::cout<<"Invalid choice. Please enter a valid choice\n";
            std::cout<<endl<<"--------------------------------------------------------------------------------------"<<endl<<endl;
            break;
        }
    }
    return 0;
}
