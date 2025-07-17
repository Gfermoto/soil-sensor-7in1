/**
 * @file IMqttConnectionManager.h
 * @brief Интерфейс для менеджера MQTT соединений
 * @details Определяет контракт для управления MQTT подключениями
 */

#ifndef IMQTT_CONNECTION_MANAGER_H
#define IMQTT_CONNECTION_MANAGER_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

/**
 * @brief Интерфейс для менеджера MQTT соединений
 *
 * Определяет контракт для управления подключениями к MQTT брокеру,
 * включая подключение, публикацию, подписку и обработку ошибок.
 */
class IMqttConnectionManager
{
   public:
    virtual ~IMqttConnectionManager() = default;

    /**
     * @brief Подключается к MQTT брокеру
     *
     * @return true если подключение успешно, false в противном случае
     */
    virtual bool connect() = 0;

    /**
     * @brief Отключается от MQTT брокера
     */
    virtual void disconnect() = 0;

    /**
     * @brief Проверяет статус подключения
     *
     * @return true если подключен, false в противном случае
     */
    virtual bool isConnected() const = 0;

    /**
     * @brief Публикует сообщение в топик
     *
     * @param topic Топик для публикации
     * @param payload Содержимое сообщения
     * @param retained Флаг сохранения сообщения
     * @return true если публикация успешна, false в противном случае
     */
    virtual bool publish(const String& topic, const String& payload, bool retained = false) = 0;

    /**
     * @brief Подписывается на топик
     *
     * @param topic Топик для подписки
     * @return true если подписка успешна, false в противном случае
     */
    virtual bool subscribe(const String& topic) = 0;

    /**
     * @brief Отписывается от топика
     *
     * @param topic Топик для отписки
     * @return true если отписка успешна, false в противном случае
     */
    virtual bool unsubscribe(const String& topic) = 0;

    /**
     * @brief Обрабатывает входящие сообщения
     *
     * Должен вызываться в основном цикле для обработки входящих MQTT сообщений
     */
    virtual void loop() = 0;

    /**
     * @brief Получает последнюю ошибку
     *
     * @return Строка с описанием последней ошибки
     */
    virtual String getLastError() const = 0;

    /**
     * @brief Устанавливает callback для обработки входящих сообщений
     *
     * @param callback Функция обратного вызова
     */
    virtual void setCallback(std::function<void(char*, uint8_t*, unsigned int)> callback) = 0;
};

#endif  // IMQTT_CONNECTION_MANAGER_H
