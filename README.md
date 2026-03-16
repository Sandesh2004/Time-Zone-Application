# Global Time Zone Console Application

A **C++ console-based application** that allows users to view and convert time across different global time zones.  
The application reads time zone information from a file and provides features like live world clock, time conversion, favorites management, and city search.

This project demonstrates **Object-Oriented Programming (OOP)** concepts, modular design, file handling, and time manipulation using C++.

---

## Features

### 1. View Current Time
- Displays the **current date and time** of a selected city.
- Supports **Daylight Saving Time (DST)** adjustments.

### 2. Time Conversion
- Convert **date and time from one time zone to another**.
- Automatically adjusts the date when crossing midnight.

### 3. Favorites
- Save frequently used cities to **favorites**.
- View time for **all favorite cities quickly**.

### 4. Search City
- Search cities available in the time zone database.

### 5. Live World Clock
- Displays real-time clocks for multiple cities.
- Updates every second.
- Press **Q** to exit the live clock view.

### 6. Daylight Saving Time Toggle
- Enable or disable DST adjustments globally.

---

## Technologies Used

- **C++**
- **Standard Template Library (STL)**
- File Handling
- Object-Oriented Programming
- Console-based UI
- Time manipulation using `ctime`

---

## Project Structure
```
TimeZoneApp
│
├── main.cpp
│
├── Application.h
├── Application.cpp
│
├── TimeZone.h
├── TimeZone.cpp
│
├── TimeZoneDatabase.h
├── TimeZoneDatabase.cpp
│
├── TimeConverter.h
├── TimeConverter.cpp
│
├── FavoriteManager.h
├── FavoriteManager.cpp
│
├── zones.txt
├── favorites.txt
```
---

### Description of Key Components

| File | Purpose |
|-----|-----|
| `TimeZone` | Data model representing a timezone |
| `TimeZoneDatabase` | Loads and manages timezone data |
| `TimeConverter` | Handles time calculations and formatting |
| `FavoriteManager` | Reads/writes favorite cities |
| `Application` | Handles user interaction and menu |
| `zones.txt` | Database of cities and timezone offsets |
| `favorites.txt` | Stores user favorite cities |

---

## Design Concepts Used

### Singleton Pattern
`TimeZoneDatabase` uses the **Singleton design pattern** to ensure only one instance of the database exists.

### Separation of Concerns
Each class has a **single responsibility**, improving maintainability and readability.

### Modular Architecture
The project is divided into multiple **header and source files** following professional C++ practices.

---

## How to Compile and Run

### Compile

```bash
g++ main.cpp Application.cpp TimeZone.cpp TimeZoneDatabase.cpp TimeConverter.cpp FavoriteManager.cpp -o timezone
```

### Run

```bash
.\timezone or timezone.exe
```

### Example Menu
```
========== GLOBAL TIME ZONE ==========

1 View Current Time
2 Convert Time
3 Toggle DST
4 Add Favorite
5 View Favorites
6 Search City/Country
7 Live World Clock
8 Exit
```

### Example Output
```
====================================
City: Tokyo (Japan)
Zone: JST
Date: 13-03-2026
Time: 18:22:31
DST: Not Applied
====================================
```
