# UDRMS
University Dormitory and Restaurant Management System
# UDRMS — University Dormitory & Restaurant Management System

> A smart management platform for ENSIA's student dormitories and restaurant services.  
> Built with **C++** and **Qt Creator** — ENSIA 2025/2026

---

## 📋 Overview

UDRMS is a desktop application developed as an Object-Oriented Programming project. It provides two separate portals — one for administrators and one for students — to manage all aspects of university dormitory life: room assignments, restaurant menus, student messaging, and more.

---

## ✨ Features

### 🔐 Authentication
- Role selection screen: **Administrator** or **Student**
- Admin login with password protection
- Student login with full name and student ID

### 🖥️ Administrator Portal
| Page | Description |
|------|-------------|
| **Dashboard** | KPI cards (total students, dormitories, rooms, available rooms), occupancy per dormitory, system information |
| **Students** | Enroll students into rooms, assign dormitory and room, view enrolled student list, remove students |
| **Dormitories** | Add new dormitories, add rooms to dormitories, view dormitory list with capacity and occupancy |
| **Restaurant** | Set daily meal menus (Breakfast / Lunch / Dinner) with main dish, salad, fruit, juice, and soup for each dormitory |
| **Messages** | View messages sent by students, reply to messages, mark messages as resolved |

### 🎓 Student Portal
| Page | Description |
|------|-------------|
| **My Room** | View personal accommodation details: student ID, name, academic year, dormitory, room number, capacity, and roommates |
| **Today's Menu** | View the daily restaurant menu set by administration |
| **Available Rooms** | Browse rooms with free spaces across all dormitories |
| **Send Message** | Send maintenance requests or room change requests to administration; view admin replies |
| **Contacts** | View key dormitory contacts (Chef de Pavillon, Maintenance, Restaurant Manager, Medical Infirmary, Security, Administration) |

---


## 🧱 OOP Class Hierarchy

```
University
└── Dormitory
    ├── Room
    │   └── Student
    └── Restaurant
        └── Meal (struct)
            ├── Breakfast  { mainDish, salad, fruit, juice, soup }
            ├── Lunch      { mainDish, salad, fruit, juice, soup }
            └── Dinner     { mainDish, salad, fruit, juice, soup }
```## Author
Brahimi Racha group 6
