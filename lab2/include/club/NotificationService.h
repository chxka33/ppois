#pragma once

#include <string>
#include <vector>

class Member;
class GroupClass;
class Invoice;
class Date;

// Отправка уведомлений клиентам (хранит журнал отправленных сообщений).
class NotificationService {
public:
    explicit NotificationService(int reminderDays);

    int getReminderDays() const;
    const std::vector<std::string>& getSentMessages() const;

    bool notifyMembershipExpiry(const Member& member, const Date& today);
    int notifyClassCancelled(const GroupClass& groupClass);
    bool notifyOverdueInvoice(const Invoice& invoice, const Date& today);
    bool sendBirthdayGreeting(const Member& member, const Date& today);
    void clearLog();

private:
    void send(const std::string& recipient, const std::string& text);

    int reminderDays_;
    std::vector<std::string> sentMessages_;
};
