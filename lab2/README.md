### Сборка и запуск

```bash
cmake -B build
cmake --build build
```

- Запуск интерактивного CLI-приложения:
  ```bash
  ./build/sports_club_cli
  ```
- Запуск модульных тестов (Catch2):
  ```bash
  ./build/sports_club_test
  ```
- Сборка с покрытием кода (`ENABLE_COVERAGE`):
  ```bash
  cmake -B build -DENABLE_COVERAGE=ON
  cmake --build build
  ```

## Lab2 Class Model

Формат строки: `Класс | число полей | число методов | связанные классы`.

| Класс | Поля | Методы | Ассоциации (связанные классы) |
|---|---|---|---|
| AccessCard | 6 | 4 | Date |
| Address | 4 | 3 | — |
| Administrator | 2 | 4 | SportsClub, Date, Discount, AccessCard, Member, Employee |
| BodyMetrics | 4 | 2 | Date |
| Booking | 6 | 2 | TimeSlot, Facility, Member |
| BookingSystem | 2 | 5 | TimeSlot, Facility, TennisCourt, Member, Booking |
| Cleaner | 2 | 3 | Date, Facility, Employee |
| Coach | 6 | 8 | Date, Member, Employee, TrainingProgram |
| Date | 3 | 12 | — |
| Discount | 5 | 3 | Date |
| Employee | 5 | 5 | Date, Person |
| Equipment | 7 | 4 | Date |
| Exercise | 5 | 4 | — |
| ExerciseBike | 2 | 3 | Date, Equipment |
| Expense | 4 | 1 | Date |
| Facility | 8 | 8 | Date, Member |
| FinanceDepartment | 3 | 7 | Date, MaintenanceRecord, Expense, Payment |
| GroupClass | 8 | 6 | Date, TimeSlot, Facility, Member, Coach, Workout |
| GuestPass | 5 | 2 | Date |
| Gym | 2 | 4 | Equipment, Facility |
| HealthRecord | 2 | 5 | Date, MedicalCertificate |
| Invoice | 7 | 7 | Date, Discount, InvoiceItem, Payment, Member |
| InvoiceItem | 3 | 1 | — |
| Locker | 4 | 3 | — |
| LockerRoom | 1 | 4 | Facility, Locker |
| LoyaltyProgram | 3 | 3 | — |
| MaintenanceRecord | 5 | 2 | Date, Equipment |
| Match | 6 | 2 | Date, Team |
| MedicalCertificate | 5 | 4 | Date |
| MedicalOfficer | 2 | 2 | Date, MedicalCertificate, Member, Employee |
| Member | 8 | 11 | Date, Person, Facility, Invoice, AccessCard, HealthRecord, LoyaltyProgram, Membership, MembershipPlan, ProgressTracker, Visit |
| Membership | 7 | 8 | Date, MembershipPlan |
| MembershipPlan | 7 | 5 | Date, Discount |
| NotificationService | 2 | 5 | Date, Invoice, Member, GroupClass |
| Payment | 6 | 4 | Date |
| Payroll | 2 | 4 | Date, FinanceDepartment, Employee |
| Person | 5 | 5 | Date |
| Product | 4 | 4 | — |
| ProgressTracker | 2 | 2 | BodyMetrics |
| Receptionist | 3 | 4 | Date, Facility, GuestPass, LoyaltyProgram, Member, Visit, Employee |
| ReportGenerator | 2 | 4 | SportsClub, Date, Gym, FinanceDepartment, Schedule |
| Review | 6 | 2 | Date, Coach |
| Sauna | 2 | 2 | Facility |
| Schedule | 2 | 5 | Date, TimeSlot, Facility, Coach, GroupClass, TrainingSession |
| SportsClub | 10 | 12 | Address, Date, Facility, FinanceDepartment, LoyaltyProgram, Member, MembershipPlan, Employee, BookingSystem, Schedule |
| SportsShop | 2 | 4 | Product, LoyaltyProgram, Member |
| SwimmingPool | 4 | 3 | Facility |
| Team | 8 | 8 | Member, Coach |
| Technician | 2 | 2 | Date, Equipment, MaintenanceRecord, Employee |
| TennisCourt | 3 | 3 | Facility |
| TimeSlot | 3 | 3 | Date |
| Tournament | 8 | 4 | Date, FinanceDepartment, Match, Team |
| TrainingProgram | 5 | 5 | Coach, Exercise |
| TrainingSession | 7 | 4 | TimeSlot, Facility, Member, Coach, TrainingProgram |
| Treadmill | 3 | 2 | Date, Equipment |
| Visit | 6 | 1 | Date, Facility |
| Workout | 4 | 2 | — |

### Исключения (19)

- SportsClubException
- AccessDeniedException
- BookingConflictException
- ClassFullException
- CoachUnavailableException
- EmployeeNotFoundException
- EquipmentBrokenException
- FacilityFullException
- FacilityNotFoundException
- InsufficientFundsException
- InvalidDataException
- InvalidDateException
- InvalidPinException
- MedicalClearanceException
- MemberNotFoundException
- MembershipExpiredException
- MembershipFrozenException
- OutOfStockException
- PaymentException

### Итоговая статистика

| Показатель | Значение |
|---|---|
| Классы | 57 (76 с учётом классов исключений) |
| Поля | 250 |
| Поведения | 241 (всего методов с геттерами/сеттерами — 519) |
| Ассоциации | 146 |
| Исключения | 19 |

