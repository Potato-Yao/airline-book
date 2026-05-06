# project report

## requirements analysis

The airline booking system's highest-priority requirement is stability. It must ensure the data is correct, and invalid operations must not cause crashes. So the crud logic should be an important part.

The system should also be efficient, which implies that we should use suitable data structures to maintain data in memory and make it work fast.

Also, data should be stored locally. I chose CSV for data storage because it is simple to read and modify.

Under these guarantees, the system should also have an easy-to-use interface.

## module design

### dao

This module contains basic data access objects, which should not have too much logic, but should just record data.

The `flight` contains flight info, including the ID, start station, terminal station, departure time, and maximum ticket number.

The `db\_action\_cell` contains database cell actions: what to do and which cell to update.

### database

This module is the implementation of the CSV manager. To keep coupling minimal, the manager does not care about the flight data layout, but only records changeable `titles`.

The interesting part of the manager is that it supports doing a lot of `change` operations at once. This is because I/O operations cost much more time than RAM operations, and the only way to change a given cell is to read from the beginning until reaching the query condition.

The manager supports two initialization modes: `load` to load the file if it already exists, and `override` to override the file no matter what.

### manager

This module contains some upper-level encapsulation.

The `database\_manager` provides flight-related functions for the database. The name of the flights database is also hard-coded here. This manager supports listing all flights, querying flights by given conditions, and booking or refunding tickets. The more important part is that it holds all flight data in a vector, which makes queries much faster since they happen in memory. So the key point is that it must make sure the data in memory is exactly the same as the data on disk, which is not hard to implement thanks to the good function design.

### cli

This is the interface of the system. I wrote a simple state machine here to parse input from users.

The CLI uses `$` to indicate that you can type commands here. If you type a command like `book`, which can take multiple lines, the prompt symbol becomes `> `, which is the same as Bash. The CLI supports error handling: if you type an unknown command, it shows an error in red. You may notice the error message is sent by stdout but not stderr. This is because stderr flushes the buffer every time we call it, making prompt symbol output look wrong. So I use stdout with color instead.

### result display

Here are some typical cases:

![](./imgs/0.png)
![](./imgs/1.png)
![](./imgs/2.png)
![](./imgs/3.png)
![](./imgs/4.png)

