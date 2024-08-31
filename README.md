# Car Rental System - CS253_cpp_assignment

This assignment involves a Car Rental System.
The program has been implemented in C++ programming language and file handling has been used for storing the data of the program.
There are 3 csv files namely car.csv, customer.csv and employee.csv that stores the records of cars, customers and employees respectively.

Initially, the csv files contains 5 cars, 5 customers (Eg: userID c0 has password 123) and 5 employees (Eg: userID e0 has password 123). The program initially begins with one manager with **ID: m0, Name: manager, Password: 123**. Other managers can be added in runtime but since they are not stored in a database or file, record of managers initiates with the above whenever the program is restarted.

This is an Object Oriented Program that has a car class, a user class, a manager and a customer class. The manager an customer class are derived from the user class. Due to similar operations of employees and customers, they have been implemented using the same customer class. ID, name and password of a users and ID and name of cars are set as protected and private variables respectively.

The code for the software has been written in two cpp files namely:
1. classes.cpp : contains the definition of the classes and the functions associated with the classes
2. car_management.cpp : contains the various functions of the program and the main function.
To run the program, we have to run the car_management.cpp file.

## Attributes of the classes:
1. **Car class:** ID, Name, Model, Color, Condition, Is_booked, Due_date, Damage, Renter_ID, Renter_name
2. **User class:** ID, Name, Password
3. **Customer class (derived from user class):** ID, Name, Password, Fine, Record, Discount (0% for customer and 15% for employee), Number_of_rented_cars
4. **Manager class (derived from user class):** ID, Name, Password

Data is stored in the csv files in the same order as given above.

The record of the customer/employee is an integer between 0 to 5. A person's record decreases by 1 when he returns a car after the due date or caused some damage to the car. It also denotes the maximum number of cars that can be rented by a customer/employee. A new customer's or employee's record is set to the average record of customers or employees respectively.

## Functions available in the car rental system:
### Manager:
1. Show all cars
2. Show all customers
3. Show all employees
4. Show all managers
5. Add a car
6. Remove a car
7. Update a car
8. Add a customer
9. Remove a customer
10. Update a customer
11. Add an employee
12. Remove an employee
13. Update an employee
14. Add a manager
15. Remove a manager
16. Update a manager
17. Search for a customer - Search by ID, Name
18. Search for an employee - Search by ID, Name
19. Search for a manager - Search by ID, Name
20. Search for a car - Search by ID, Name, Model, Color, Condition, Renter_ID
21. Show rented cars
22. Logout
### Customer:
1. Show available cars
2. Show rented cars
3. Rent a car - asks for the number of days you want to rent the car for and updates the parameters accordingly in csv files. Calculates the fare (₹1000 per day) of these days and displays it.
4. Return a car - checks the due date and asks for any damage in the car
5. Show fine
6. Clear fine
7. View profile
8. Logout
### Employee:
Since employee and customer have been implemented by the same class, both have the same functionalities

## Structure of the Program:
### Starting options:
1. Login: gives 3 login options namely - login as customer, login as employee and login as manager
2. Signup: gives 2 signup options namely - signup as customer and signup as employee
3. Exit: Exits the program

While signing up, one has to give his name and password and the system generates a unique userID and displays it. While logging in, one has to give his UserID (Eg: c0 or e0 or m0) and password that is verified and the action proceeds. After signing up, the employee or customer is given a unique ID. The maneger, customer or employee can access the functions of the program once he/she has logged in.

### Testing the fine system:
The fine system gives a fine to user if there is a damage in the car (₹200 per damage) or if the user return the car after the due date (₹1200 per late day). For testing the late return fine feature, we can manually change the due date of a car (in format dd-mm-yyyy) in 'car.csv' after booking the car for a user and then try returning the car from that user. The fine will be added to the fine of the user.