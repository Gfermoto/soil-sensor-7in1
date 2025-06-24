#pragma once

// Centralized API route strings for the web layer. Use these constants instead of hard-coded literals.

#define API_ROOT           "/api/v1"

// Sensor data
#define API_SENSOR         API_ROOT "/sensor"

// System
#define API_SYSTEM         API_ROOT "/system"
#define API_SYSTEM_HEALTH  API_SYSTEM "/health"
#define API_SYSTEM_STATUS  API_SYSTEM "/status"
#define API_SYSTEM_RESET   API_SYSTEM "/reset"
#define API_SYSTEM_REBOOT  API_SYSTEM "/reboot"

// Config
#define API_CONFIG_EXPORT  API_ROOT "/config/export" 