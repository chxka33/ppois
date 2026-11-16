#include "club/NotificationService.h"

#include "common/Date.h"
#include "training/GroupClass.h"
#include "finance/Invoice.h"
#include "members/Member.h"

NotificationService::NotificationService(int reminderDays) : reminderDays_(reminderDays) {}

int NotificationService::getReminderDays() const { return reminderDays_; }
const std::vector<std::string>& NotificationService::getSentMessages() const { return sentMessages_; }

bool NotificationService::notifyMembershipExpiry(const Member& member, const Date& today) {
    Membership* m = member.getMembership();
    if (!m || !m->isActive(today)) {
        return false;
    }
    int remaining = m->daysRemaining(today);
    if (remaining <= reminderDays_ && remaining >= 0) {
        send(member.getEmail(), "Your membership expires in " + std::to_string(remaining) + " days.");
        return true;
    }
    return false;
}

int NotificationService::notifyClassCancelled(const GroupClass& groupClass) {
    int count = 0;
    for (const auto* m : groupClass.getParticipants()) {
        if (m) {
            send(m->getEmail(), "Group class " + groupClass.getName() + " has been cancelled.");
            count++;
        }
    }
    return count;
}

bool NotificationService::notifyOverdueInvoice(const Invoice& invoice, const Date& today) {
    if (invoice.isOverdue(today)) {
        if (invoice.getMember()) {
            send(invoice.getMember()->getEmail(), "Invoice " + invoice.getNumber() + " is overdue.");
            return true;
        }
    }
    return false;
}

bool NotificationService::sendBirthdayGreeting(const Member& member, const Date& today) {
    if (member.isBirthday(today)) {
        send(member.getEmail(), "Happy Birthday, " + member.getFirstName() + "!");
        return true;
    }
    return false;
}

void NotificationService::clearLog() {
    sentMessages_.clear();
}

void NotificationService::send(const std::string& recipient, const std::string& text) {
    sentMessages_.push_back(recipient + ": " + text);
}
