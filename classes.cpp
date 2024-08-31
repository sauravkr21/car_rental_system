#include <iostream>
#include <vector>
#include <string>
using namespace std;

class car  // id, name, model, color, condition, is_booked, due_date, damage, renter_id, renter_name
{
    private:
        string id, name;
    public:
        string model;
        string color;
        string condition;
        int is_booked;
        int due_date;
        int damage;
        string renter_id;
        string renter_name;
        static string file;
        static int car_id_count;
        static int cost_per_day;
        static int fine_per_day;
        static int damage_cost;

        car(string id, string name, string model, string color, string condition, int is_booked, int due_date, int damage, string renter_id, string renter_name)
        {
            this->id = id;
            this->name = name;
            this->model = model;
            this->color = color;
            this->condition = condition;
            this->is_booked = is_booked;
            this->due_date = due_date;
            this->damage = damage;
            this->renter_id = renter_id;
            this->renter_name = renter_name;
        }

        void set_val(string id, string name, string model, string color, string condition, int is_booked, int due_date, int damage, string renter_id, string renter_name)
        {
            this->id = id;
            this->name = name;
            this->model = model;
            this->color = color;
            this->condition = condition;
            this->is_booked = is_booked;
            this->due_date = due_date;
            this->damage = damage;
            this->renter_id = renter_id;
            this->renter_name = renter_name;
        }
        void add_car();
        void remove_car();
        void update_car();
};

class user
{
    protected:
        string id, name, password;
};

class customer: public user  // id, name, password, fine, record, discount, number_of_rented_cars
{
    public:
        int record;
        int fine;
        int discount;
        int number_of_rented_cars;

        static int customer_avg_record, number_of_customers;
        static int employee_avg_record, number_of_employees;
        static int customer_id_count, employee_id_count;
        static string customer_file;
        static string employee_file;

        customer(string id, string name, string password, int fine, int record, int discount, int number_of_rented_cars)
        {
            this->id = id;
            this->name = name;
            this->password = password;
            this->fine = fine;
            this->record = record;
            this->discount = discount;
            this->number_of_rented_cars = number_of_rented_cars;
        }
        void show_available_cars();
        void show_rented_cars();
        void rent_car();
        void return_car();
        void clear_fine();
        void view_profile();
};

class manager: public user
{
    public:
        static int manager_id_count;
        manager(string id, string name, string password)
        {
            this->id = id;
            this->name = name;
            this->password = password;
        }
        void show_all_cars();
        void show_all_customers();
        void show_all_employees();
        void show_all_managers(vector<vector<string>> &managers);

        void add_customer();
        void remove_customer();
        void update_customer();
        void add_employee();
        void remove_employee();
        void update_employee();
        void add_manager(vector<vector<string>> &managers);
        void remove_manager(vector<vector<string>> &managers);
        void update_manager(vector<vector<string>> &managers);
        void search_customer_by_id();
        void search_customer_by_name();
        void search_employee_by_id();
        void search_employee_by_name();
        void search_manager_by_id(vector<vector<string>> &managers);
        void search_manager_by_name(vector<vector<string>> &managers);
        void search_car_by_id();
        void search_car_by_name();
        void search_car_by_model();
        void search_car_by_color();
        void search_car_by_condition();
        void search_car_by_renter_ID();
        void search_car_by_renter_name();
        void show_rented_cars();
};