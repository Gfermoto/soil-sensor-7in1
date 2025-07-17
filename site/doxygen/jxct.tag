<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.13.2" doxygen_gitid="26342b775ea25e6fefb53220926b20702c56fcb3">
  <compound kind="file">
    <name>advanced_filters.h</name>
    <path>include/</path>
    <filename>advanced__filters_8h.html</filename>
    <includes id="modbus__sensor_8h" name="modbus_sensor.h" local="yes" import="no" module="no" objc="no">modbus_sensor.h</includes>
    <namespace>AdvancedFilters</namespace>
  </compound>
  <compound kind="file">
    <name>basic_sensor_adapter.h</name>
    <path>include/</path>
    <filename>basic__sensor__adapter_8h.html</filename>
    <includes id="ISensor_8h" name="ISensor.h" local="yes" import="no" module="no" objc="no">ISensor.h</includes>
    <class kind="class">BasicSensorAdapter</class>
  </compound>
  <compound kind="file">
    <name>ICropRecommendationEngine.h</name>
    <path>include/business/</path>
    <filename>ICropRecommendationEngine_8h.html</filename>
    <includes id="sensor__compensation_8h" name="sensor_compensation.h" local="yes" import="no" module="no" objc="no">../sensor_compensation.h</includes>
    <class kind="struct">RecValues</class>
    <class kind="class">ICropRecommendationEngine</class>
    <member kind="enumeration">
      <type></type>
      <name>Season</name>
      <anchorfile>ICropRecommendationEngine_8h.html</anchorfile>
      <anchor>ace3a72289effba7c41de4a566bd3c300</anchor>
      <arglist></arglist>
      <enumvalue file="ICropRecommendationEngine_8h.html" anchor="ace3a72289effba7c41de4a566bd3c300a2e2302818a996993c08f2f07c9606e79">SPRING</enumvalue>
      <enumvalue file="ICropRecommendationEngine_8h.html" anchor="ace3a72289effba7c41de4a566bd3c300acc49522c59ab153ffcd4e38b7efc691d">SUMMER</enumvalue>
      <enumvalue file="ICropRecommendationEngine_8h.html" anchor="ace3a72289effba7c41de4a566bd3c300a8e8f3c8204a6f6d9cec7f76a2b8928af">AUTUMN</enumvalue>
      <enumvalue file="ICropRecommendationEngine_8h.html" anchor="ace3a72289effba7c41de4a566bd3c300a832294ab11474e3f807c5eaa92b63058">WINTER</enumvalue>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>EnvironmentType</name>
      <anchorfile>ICropRecommendationEngine_8h.html</anchorfile>
      <anchor>a1de7cecb71b1f30091409b059b24e8d6</anchor>
      <arglist></arglist>
      <enumvalue file="ICropRecommendationEngine_8h.html" anchor="a1de7cecb71b1f30091409b059b24e8d6ae54de3be93a9c7071403faf31de7ad84">OUTDOOR</enumvalue>
      <enumvalue file="ICropRecommendationEngine_8h.html" anchor="a1de7cecb71b1f30091409b059b24e8d6a0f9c4a1832a458e60bb416897c19890a">GREENHOUSE</enumvalue>
      <enumvalue file="ICropRecommendationEngine_8h.html" anchor="a1de7cecb71b1f30091409b059b24e8d6a0ab3790f669b52c65c734a1e7b68f8e1">INDOOR</enumvalue>
    </member>
  </compound>
  <compound kind="file">
    <name>ISensorCalibrationService.h</name>
    <path>include/business/</path>
    <filename>ISensorCalibrationService_8h.html</filename>
    <includes id="sensor__compensation_8h" name="sensor_compensation.h" local="yes" import="no" module="no" objc="no">../sensor_compensation.h</includes>
    <includes id="validation__utils_8h" name="validation_utils.h" local="yes" import="no" module="no" objc="no">../validation_utils.h</includes>
    <class kind="class">ISensorCalibrationService</class>
  </compound>
  <compound kind="file">
    <name>ISensorCompensationService.h</name>
    <path>include/business/</path>
    <filename>ISensorCompensationService_8h.html</filename>
    <includes id="sensor__compensation_8h" name="sensor_compensation.h" local="yes" import="no" module="no" objc="no">../sensor_compensation.h</includes>
    <includes id="validation__utils_8h" name="validation_utils.h" local="yes" import="no" module="no" objc="no">../validation_utils.h</includes>
    <class kind="class">ISensorCompensationService</class>
  </compound>
  <compound kind="file">
    <name>business_services.h</name>
    <path>include/</path>
    <filename>business__services_8h.html</filename>
    <includes id="ICropRecommendationEngine_8h" name="ICropRecommendationEngine.h" local="yes" import="no" module="no" objc="no">business/ICropRecommendationEngine.h</includes>
    <includes id="ISensorCalibrationService_8h" name="ISensorCalibrationService.h" local="yes" import="no" module="no" objc="no">business/ISensorCalibrationService.h</includes>
    <includes id="ISensorCompensationService_8h" name="ISensorCompensationService.h" local="yes" import="no" module="no" objc="no">business/ISensorCompensationService.h</includes>
    <member kind="function">
      <type>ICropRecommendationEngine &amp;</type>
      <name>getCropEngine</name>
      <anchorfile>business__services_8h.html</anchorfile>
      <anchor>a9e42803bf759fbcfcce0c55a600f6590</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>ISensorCalibrationService &amp;</type>
      <name>getCalibrationService</name>
      <anchorfile>business__services_8h.html</anchorfile>
      <anchor>af5557d1c7a1e7954fd87d6df62d881c9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>ISensorCompensationService &amp;</type>
      <name>getCompensationService</name>
      <anchorfile>business__services_8h.html</anchorfile>
      <anchor>a534645a7c06e83e207c21a8c48924248</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>debug.h</name>
    <path>include/</path>
    <filename>debug_8h.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_PRINT</name>
      <anchorfile>debug_8h.html</anchorfile>
      <anchor>af6551f0b4c3964c3128f808e7bf987c7</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_PRINTLN</name>
      <anchorfile>debug_8h.html</anchorfile>
      <anchor>a8e30ff0704664fcb13dabf0c4dc5c8f5</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_PRINTF</name>
      <anchorfile>debug_8h.html</anchorfile>
      <anchor>aa97e8dc28005a1241103bef65128944b</anchor>
      <arglist>(fmt,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_PRINT_VAR</name>
      <anchorfile>debug_8h.html</anchorfile>
      <anchor>a4c9c6f1fcf820b8f001ceb637faf6e16</anchor>
      <arglist>(name, value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_PRINT_INT</name>
      <anchorfile>debug_8h.html</anchorfile>
      <anchor>a604f66fd967d8aca0910fb8affdff324</anchor>
      <arglist>(name, value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_PRINT_FLOAT</name>
      <anchorfile>debug_8h.html</anchorfile>
      <anchor>a35b442170ee844eabf46efd41cd58e17</anchor>
      <arglist>(name, value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERROR_PRINT</name>
      <anchorfile>debug_8h.html</anchorfile>
      <anchor>aefd58cef04351b40871f9aa0c84a895a</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERROR_PRINTLN</name>
      <anchorfile>debug_8h.html</anchorfile>
      <anchor>aab934c3f2345fdd7f8ca02ff2c332f60</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERROR_PRINTF</name>
      <anchorfile>debug_8h.html</anchorfile>
      <anchor>aa7ede35711728c53eb42c1c5c0def9e7</anchor>
      <arglist>(fmt,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INFO_PRINT</name>
      <anchorfile>debug_8h.html</anchorfile>
      <anchor>ada3de6ba49d97903fbf36000cc99d55d</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INFO_PRINTLN</name>
      <anchorfile>debug_8h.html</anchorfile>
      <anchor>a693063ba84ad2119e7f3ea1b97947055</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INFO_PRINTF</name>
      <anchorfile>debug_8h.html</anchorfile>
      <anchor>a968ff3408e99f72ecde7095446a3af1e</anchor>
      <arglist>(fmt,...)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>debug_optimized.h</name>
    <path>include/</path>
    <filename>debug__optimized_8h.html</filename>
    <includes id="jxct__constants_8h" name="jxct_constants.h" local="yes" import="no" module="no" objc="no">jxct_constants.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_ENABLED</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a7d2ae674cad5299a52b0e7dceac10087</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_PRINT</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>af6551f0b4c3964c3128f808e7bf987c7</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_PRINTLN</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a8e30ff0704664fcb13dabf0c4dc5c8f5</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_PRINTF</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>aa97e8dc28005a1241103bef65128944b</anchor>
      <arglist>(fmt,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_MODBUS_PRINT</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a54fbc5700cffd26a61494ed6a400cc45</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_MODBUS_PRINTLN</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>ae53267bde79e149c80877da5ae362867</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_MODBUS_PRINTF</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>ae4d36024b895e5f526ef89071605badb</anchor>
      <arglist>(fmt,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_MQTT_PRINT</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a97da1769619ef98d76b61c44d397f9e3</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_MQTT_PRINTLN</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>ad5b6145964771adcf15eabf41b7e3e01</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_MQTT_PRINTF</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>aaa225a0f27c84d8f7eb7747758be4732</anchor>
      <arglist>(fmt,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_WIFI_PRINT</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a2408c7052942f2a0ac696db06146ef10</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_WIFI_PRINTLN</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a63cac6dc284968cd7eba8e2f7c28f052</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_WIFI_PRINTF</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>ad77ad7c975f7f577f0f69b2207f28b68</anchor>
      <arglist>(fmt,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_MQTT_DELTA</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a8423cfacd2a82de4ee8647dae55f10c5</anchor>
      <arglist>(fmt,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_MQTT_DNS</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a78185c8405fcd043bb18acaf5373de41</anchor>
      <arglist>(fmt,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_MQTT_HA</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a6cdde71e1eb3d615f0c74da6ea3e625c</anchor>
      <arglist>(fmt,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_MODBUS_TX</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a374ac3552d0860c64fdc74a97b543b4e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_MODBUS_RX</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>aac8300f6f06573e98f788bd79619c046</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_MODBUS_MOVING_AVG</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>ab166ebe03f665d58b3593ddce59bb560</anchor>
      <arglist>(fmt,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_MAIN_BATCH</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a447445ef4f51447616d082dce0033497</anchor>
      <arglist>(msg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEBUG_MAIN_BUTTON</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a839e542538d5bd02cba6778be55a6eb9</anchor>
      <arglist>(msg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MIGRATE_DEBUG_PRINTF</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a10b2de8934fb63e17b4d31b54febf824</anchor>
      <arglist>(fmt,...)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MIGRATE_DEBUG_PRINTLN</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a21afe95f02a9a06fc9854864a1a9d4bd</anchor>
      <arglist>(msg)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CRITICAL_DEBUG_PRINT</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a5bf4ca35d3d5283648b65b646a9d4710</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CRITICAL_DEBUG_PRINTLN</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>aca5049ad9737772e69632c4ffde5661b</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CRITICAL_DEBUG_PRINTF</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>afd5d375337ca9d6bebf6b03bc3b7af43</anchor>
      <arglist>(fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>debugConditionalPrint</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a3b1c1240a45bf11367e496c24a1c354d</anchor>
      <arglist>(int level, const char *format,...)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>debugPrintHexBuffer</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a106b655aa8faea8942757638731ebf0d</anchor>
      <arglist>(const char *prefix, const uint8_t *buffer, size_t length)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>debugPrintStatus</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a4002fe059558510a239058a9dda387a6</anchor>
      <arglist>(const char *module, bool status, const char *details=nullptr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>debugStatsInit</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>ae3aab77ba55ab7c94378c6110d9a3050</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>debugStatsIncrement</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a795c0e9d5464893e565a264b0fb6e75e</anchor>
      <arglist>(const char *category)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>debugStatsPrint</name>
      <anchorfile>debug__optimized_8h.html</anchorfile>
      <anchor>a748fe53bf9a9627f05f3d3697ff26971</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>IMqttConnectionManager.h</name>
    <path>include/infrastructure/</path>
    <filename>IMqttConnectionManager_8h.html</filename>
    <class kind="class">IMqttConnectionManager</class>
  </compound>
  <compound kind="file">
    <name>ISensor.h</name>
    <path>include/</path>
    <filename>ISensor_8h.html</filename>
    <class kind="class">ISensor</class>
  </compound>
  <compound kind="file">
    <name>jxct_config_vars.h</name>
    <path>include/</path>
    <filename>jxct__config__vars_8h.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>BOOT_BUTTON</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>ac1268edc13ca85789628d69528560051</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STATUS_LED_PIN</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a089a71f836911c71b3f73fdd3b4b890b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SENSOR_READ_INTERVAL</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>aa0e54eb96835b229951b29cfe762097b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PUBLISH_INTERVAL</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a5d391ac9f5a18b48d867fe5de3e6e73b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>THINGSPEAK_INTERVAL</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>afdd40eb3ab6faed1dd66ff50cec060f9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WEB_UPDATE_INTERVAL</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a2f69cddbe3d15df4fdced0e36984cd7a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HTTP_PORT</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a0906dae4a42c1fef9ec0cd0a5212ed4a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DNS_PORT</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>ab1d3f5c08428c20521a92ca2fa20f46c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LED_FAST_BLINK_MS</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a5a148869b99db8fccfb85591d00cf94b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LED_SLOW_BLINK_MS</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a77d7dabaae0518e06c791e4d2dce6628</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WIFI_MAX_ATTEMPTS</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a670833d6bc4ebeab45f259d03c9362c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WIFI_RETRY_DELAY_MS</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a04b848298b9a35170cd46e8ef3b88464</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BUTTON_HOLD_TIME_MS</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a21bfc357c536666cbacb163b043d27ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CONFIG_SAVE_DELAY_MS</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a9ec1b82f0673d93d65a1bd26c7d4a95f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WATCHDOG_TIMEOUT_SEC</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>adfabfbdb34653df3fe5aa5c212a140a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OTA_WATCHDOG_TIMEOUT_SEC</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a34bf95207114d92aae320115352794e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DELTA_TEMPERATURE</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>aed35e9e24cf284b3d4ccc163cd2541ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DELTA_HUMIDITY</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>ae3ef5abe7c300ed5f284959dced43add</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DELTA_PH</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>ae13f5c1a3a53aa110c1d0935eb31c33e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DELTA_EC</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a60dad88b193712506f4d823c1e3d1308</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DELTA_NPK</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>ac81d514cad041a427ef91aae287508d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FORCE_PUBLISH_CYCLES</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a68dcffe45eda49259202f133335e4c39</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>JXCT_MODBUS_ID</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>ab3f5a96fb12247f8fe0f1eb350a32d90</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WIFI_AP_SSID</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a8e19d13af09f7ed0afad391730f1e3e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WIFI_AP_PASS</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a3408bc061b1888cfd751dad7b5e3d874</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>struct</type>
      <name>__attribute__</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a482c09d923aa8219c0d59569ffe2d231</anchor>
      <arglist>((packed)) Config</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>loadConfig</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>ad5ed6ddd9940c0097cc91774056df1c2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>saveConfig</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a688d00bbabd28fbaf9e0c50eca3adeae</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resetConfig</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a6fa5204bcafe1db397da4a87b1690061</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isConfigValid</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>aeb61f04cd3b4b68e8146d85c447d70d1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>Config</type>
      <name>config</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a4a8dd3a2de125b72d4fe6251a0a271b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Preferences</type>
      <name>preferences</name>
      <anchorfile>jxct__config__vars_8h.html</anchorfile>
      <anchor>a6ea06cf7b8092a0adaf07614d7ece59d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>jxct_constants.h</name>
    <path>include/</path>
    <filename>jxct__constants_8h.html</filename>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>DEFAULT_SENSOR_READ_INTERVAL</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a6434b7a387ed20c9f364ba4761763d37</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>DEFAULT_MQTT_PUBLISH_INTERVAL</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a67dcd3564182a15e00496b2cf2227ed7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>DEFAULT_THINGSPEAK_INTERVAL</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>af693c003b6fde3f116c5fd46e4c487f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>DEFAULT_NTP_UPDATE_INTERVAL</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a4a3bbfdcb5c25d9f893585031128e1f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>MODBUS_CACHE_TIMEOUT</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a2fbd8c67fb5bfd659802a9a054798e9e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>MODBUS_RETRY_DELAY</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a64d07c204cd0c8b744644a91dd935c07</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>DNS_CACHE_TTL</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a564c8f2937d070bba56544f0b63d8842</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>MQTT_RECONNECT_INTERVAL</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ae7f737a725f45e5a262a28e54944ca6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>SENSOR_JSON_CACHE_TTL</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a5885b1438361c2ca1510f82f0824b7f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>STATUS_PRINT_INTERVAL</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a59058d2a77bd18a8eb3a5618713b5c05</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>JXCT_WATCHDOG_TIMEOUT_SEC</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a7954a4e74b56f782bc40acf8db11b6a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>JXCT_BUTTON_HOLD_TIME_MS</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ad9caf8aea0daab8f3b8499c43ca27186</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>LED_BLINK_SLOW</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a3fc48c41ab4e2ebc54d84ab9471f0970</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>LED_BLINK_NORMAL</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a15af2a0475ee85df700484308c2647a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>LED_BLINK_FAST</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a1008fcce664752a9154bab72546d34af</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>DEFAULT_WEB_SERVER_PORT</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a4e987a55f2a52382b2e444e1e658eee9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>DEFAULT_MQTT_PORT</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a58f999d62665a9f819c76ab357c9733f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>DEFAULT_DNS_PORT</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a0ddacb1e739582239933c267e104d702</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr uint8_t</type>
      <name>DEFAULT_MODBUS_ADDRESS</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a7849788c3400d733d2d535754d28168a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr size_t</type>
      <name>MQTT_BUFFER_SIZE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ae7e41f8ea7923417d3f459ae63b5c2d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr size_t</type>
      <name>JSON_BUFFER_SIZE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a3ab6698eff150be75250e5066a32c343</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr size_t</type>
      <name>TOPIC_BUFFER_SIZE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>abec57d3cf3642697f86a7d58af6fbf21</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr size_t</type>
      <name>CLIENT_ID_BUFFER_SIZE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aab222817f60c41262eab017e380eb8c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr size_t</type>
      <name>HOSTNAME_BUFFER_SIZE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>abec991a26cb2aa3678ef2c898d73c233</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>WIFI_CONNECTION_ATTEMPTS</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ad6f86c92de338fa423acd76cdffc2e1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>MQTT_CONNECTION_ATTEMPTS</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a630aeb24f56d2f3f127fb27936875c9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>WIFI_CONNECTION_TIMEOUT</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ab1991b2b4038578f7511c9c38f8620a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>MODBUS_BAUD_RATE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ab6c697e400afc274c87fdee739c88ab9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr uint8_t</type>
      <name>MODBUS_DATA_BITS</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ac2d13ba33d7829a01539892112c36d55</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr uint8_t</type>
      <name>MODBUS_STOP_BITS</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ac8e91ac73efbebaf84c496121248e76d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr uint8_t</type>
      <name>MODBUS_PARITY</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a1b3a318da7b009b1f31acac87ed4b651</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr uint8_t</type>
      <name>MODBUS_MAX_RETRIES</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a5ac0d3aaefc2ee9dcae39fbf3f274e2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>MODBUS_RESPONSE_TIMEOUT</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>afadcc38ac652f71edb1585ed8bb147b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>MODBUS_FRAME_DELAY</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a90f31c1e93e5de5be24a37b91f33e2f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>SENSOR_TEMP_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aba092fd1c7310e40134f0bc95ae6547e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>SENSOR_TEMP_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a31024a54806ee2efe49cd2c99fd5a78a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>SENSOR_HUMIDITY_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a9c63ed1a23642df5d6aba976397be257</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>SENSOR_HUMIDITY_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aa79f30db9ff75390e0254a848dd0bbc3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>SENSOR_PH_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aaea5e75d8ed5103a895ab69345e51f8e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>SENSOR_PH_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a66cabb5a09a885dfe9c920415a899f25</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr uint16_t</type>
      <name>SENSOR_EC_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a246f7e543a4fed75fdbcde68614299b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr uint16_t</type>
      <name>SENSOR_EC_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a9f7c84a54d65caeac7c47ba0a2b5bf30</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr uint16_t</type>
      <name>SENSOR_NPK_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a89872d4abc1e4808f175cdbfc032f4ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr uint16_t</type>
      <name>SENSOR_NPK_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ac3c1dd8bdfb0337a839aed6c24c4d929</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>TEMP_MIN_VALID</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ae64c08f0d22e4cea72845372cc343713</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>TEMP_MAX_VALID</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a684bb530ca59feb25cd4d53925a95f86</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>HUM_MIN_VALID</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a25fdc8731a119321d9130a46daacaeb8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>HUM_MAX_VALID</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a2eba44199e27d42d3688e6c76bb5a518</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>EC_MAX_VALID</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ad6dfe79b28fcfdd9626fd937bae6660f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>NPK_MAX_VALID</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aaebb5540383c87446ec1d35469b2bd43</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>CONFIG_INTERVAL_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ad542d64e38a669ebab31fb960826587d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>CONFIG_INTERVAL_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ae156491e40c3833e81591602342fc662</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>CONFIG_THINGSPEAK_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aa3e55f930da43f8218d6bfd0677f6b60</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>CONFIG_THINGSPEAK_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ae68374e6458fafd3f4863b0daff52477</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_MQTT_PORT_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a89c9a6f88510749f53f0c2a090dbb8bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_MQTT_PORT_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a22b1acb57336de46f5c97d12fabf2c24</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr uint8_t</type>
      <name>MOVING_AVERAGE_WINDOW_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ab31422414dd55a2921ef60ae0d16b819</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr uint8_t</type>
      <name>MOVING_AVERAGE_WINDOW_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a744db6b73c6e71a1e0d24eec6bca17d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr uint8_t</type>
      <name>MOVING_AVERAGE_WINDOW_DEFAULT</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a931af3b2c8e7dd75cda910630888cb47</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>DEFAULT_DELTA_TEMPERATURE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ad33de8864e9f7d13f8b336133258c16f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>DEFAULT_DELTA_HUMIDITY</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a149120cf7777cfaaf9bbd9414e35d63b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>DEFAULT_DELTA_PH</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ac19a75d9666ffe574bfe2f6e7b080460</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>DEFAULT_DELTA_EC</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a0664f8861561dcdd9aa7ade1acf4f7d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>DEFAULT_DELTA_NPK</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a7d9929193780ad252d25a8263d03ca88</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr uint8_t</type>
      <name>DEFAULT_FORCE_PUBLISH_CYCLES</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a2dc8534888b887068b82794f37b21374</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>EXPONENTIAL_ALPHA_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a4dff4e4c96ebb028fb21ba64f7893373</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>EXPONENTIAL_ALPHA_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a7dc955e195d4fdf33052bac2663183c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>EXPONENTIAL_ALPHA_DEFAULT</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a6342a6a7cf6d926635b64cd22f19a373</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>OUTLIER_THRESHOLD_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ad9ef60a07a693cb4ef5321af4ae08490</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>OUTLIER_THRESHOLD_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ad1b401b59a1fe93e9fed6894e157944e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>OUTLIER_THRESHOLD_DEFAULT</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aa1dda9f2e7d329a6ff95c0730dc09c2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>KALMAN_PROCESS_NOISE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ad5292dca0c9bf7546d0abc1f09e6c13a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>KALMAN_MEASUREMENT_NOISE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a793d6c869289abd612cd7953b989ca1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>KALMAN_INITIAL_UNCERTAINTY</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aa41383cea07c8c0bb2d5c87edee942e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>CALIBRATION_OFFSET_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ae75ac22532ef93f83ed0b4315d02a780</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>CALIBRATION_DRIFT_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ab82645ff070a18585d5e5735dd763eb0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr uint8_t</type>
      <name>STATISTICS_WINDOW_SIZE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a0438a45eefa18b7a10a345442e23f62b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>MIN_STANDARD_DEVIATION</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a0d3cb152eac43b624bf03eb6a66cfae5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr const char *</type>
      <name>JXCT_WIFI_AP_PASS</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a4e8fd5f93ab38700d0ef3e9fb0bcc720</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr const char *</type>
      <name>WIFI_HOSTNAME_PREFIX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ae0b400fec6319db5f57919b2bb6db8f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr const char *</type>
      <name>MQTT_TOPIC_STATE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a65e0800e07fae7240df7fae6c86c1055</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr const char *</type>
      <name>MQTT_TOPIC_STATUS</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>acc0a72b67aebd88dd6ca9655e6a90c99</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr const char *</type>
      <name>MQTT_TOPIC_COMMAND</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a03f32a6f6160cac902093b418cc8fcb5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr const char *</type>
      <name>MQTT_TOPIC_AVAILABILITY</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a78d06aa7d1d0d0c5a305667281f8f0dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr const char *</type>
      <name>HASS_DISCOVERY_PREFIX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a29476cce48d3c057ff38741926ac71db</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr const char *</type>
      <name>HASS_CONFIG_SUFFIX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a4bae8e2361c67765c52dee65bad786ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr const char *</type>
      <name>HTTP_CACHE_CONTROL</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aeb72e92dcf23f4d68c0d96ff54a22794</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr const char *</type>
      <name>HTTP_CONTENT_TYPE_HTML</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a9f358c4e2a454d1cc017d4af0dd9c3a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr const char *</type>
      <name>HTTP_CONTENT_TYPE_JSON</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ae50f04cb8a3d316eb61afdb47ccb6470</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr const char *</type>
      <name>HTTP_CONTENT_TYPE_PLAIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a6a566f0a3b16c30ada2936a16ad22628</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>RESET_BUTTON_PIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a4a3aea0890a9c7d81ed2b359ee3ae200</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>JXCT_STATUS_LED_PIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a921d1345eebedb569ba46c07be781d2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>MODBUS_RX_PIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a2ec072189e916c49b5dde8db1440b000</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>MODBUS_TX_PIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a8e4eb49e6611973f5b59be908fe81587</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>MODBUS_DE_PIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aaddc26fdd8f6a829c412fa7944605132</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>MODBUS_RE_PIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aebc5c5d6e17f3fdbe17604e645e0a55b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr size_t</type>
      <name>SENSOR_TASK_STACK_SIZE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a531be895ada8bdeaed8eba22d2648973</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr size_t</type>
      <name>RESET_BUTTON_TASK_STACK_SIZE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>af5ae137d7081e66e1cd43e5c8df7de05</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr size_t</type>
      <name>WEB_SERVER_TASK_STACK_SIZE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ab6cd27ccf48b9e8ab2d17ac4074d4465</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr UBaseType_t</type>
      <name>SENSOR_TASK_PRIORITY</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ab1c81fb9d3a4331389d1c13c7f972694</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr UBaseType_t</type>
      <name>RESET_BUTTON_TASK_PRIORITY</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a2748e3aebe921d6d2f1965fb7b71bbfa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr UBaseType_t</type>
      <name>WEB_SERVER_TASK_PRIORITY</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ab03a887f37e8650b369c91aef844be7b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr size_t</type>
      <name>MAX_CONFIG_JSON_SIZE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ab07b4a0d65d4caffeae5920dd9df117a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr size_t</type>
      <name>MAX_SENSOR_JSON_SIZE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>acb9d4de13f8e8475454a6c23f1bee8c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr size_t</type>
      <name>MAX_LOG_MESSAGE_SIZE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>abc3619785e6cc8734fabbda92533ab63</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>LOG_LEVEL_ERROR</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a09dc0bc5945818a5777cc2e52a211640</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>LOG_LEVEL_WARN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a6d1ce14731a1a1d260a9b7be4e082613</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>LOG_LEVEL_INFO</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ae482be3541ac13284ca3ac530eca44e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>LOG_LEVEL_DEBUG</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a0b03dd1b6198155aa8fb66f3f0dc1413</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr bool</type>
      <name>DEBUG_MODBUS_ENABLED</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a62792f4d036bd170077004ddc2289e90</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr bool</type>
      <name>DEBUG_MQTT_ENABLED</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ab844366d6d3f7b1115fa744eacc40157</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr bool</type>
      <name>DEBUG_WIFI_ENABLED</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>afb71d0269023668dbd7b642e71513a15</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>DEFAULT_SEPARATOR_LENGTH</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>abc454bc37d61113a513356d453998d84</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>JXCT_REDIRECT_DELAY_MS</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a4f8d3f60641bdc9b079e2bd23aa6ff62</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>HTTP_OK</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a205130c68a4a47fc1f01e68815cc2283</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>HTTP_FORBIDDEN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a5e6b8ee6c05866cc28b1dd492723e28d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>HTTP_REDIRECT_TEMPORARY</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ac850ff653880faf06e57dec2fd525a18</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>HTTP_REDIRECT_PERMANENT</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a3a2afafac1a7f959cb39deda72f32547</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>HTTP_BAD_REQUEST</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ad8293a590a80876694618b34687275f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>HTTP_SEE_OTHER</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a6b403a509a6e8ba38a7d6406ac406fe4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>JSON_DOC_SMALL</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a61993429ab91b4e4241a2d8d50c33df5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>JSON_DOC_MEDIUM</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ad4bfa4cd7f651510a9f0dbb2bdfced0a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>SECONDS_IN_MINUTE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aaa84555eb10bf09088ca749ffd0be30e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>MINUTES_IN_HOUR</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a2865dd6bd8621f643a56a25572dd5072</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>HOURS_IN_DAY</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>afce6d7b07dddca700627968cbd49cf3f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>MILLISECONDS_IN_SECOND</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a8bc600732dffe403bd08187012dfd7fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>WEB_OPERATION_DELAY_MS</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a0a14e81a81ae66096399d63876a4d1c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr const char *</type>
      <name>API_VERSION_V1</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a9736db8fbca9550722fc197649a7b05d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr const char *</type>
      <name>API_VERSION_CURRENT</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ae67763a474ed770ac17a07c9f40ed318</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_VERSION_CURRENT</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>af73711cc1a28115c48b1312705ac164a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_VERSION_MIN_SUPPORTED</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>abc736911b242f167449b14cf593b6096</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr size_t</type>
      <name>OTA_BUFFER_SIZE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a22264a27d455a1be8245280540e34650</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>OTA_TIMEOUT</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a4e82d246ba3af051580c6dc37dcaf8a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr const char *</type>
      <name>OTA_UPDATE_URL_TEMPLATE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ac2eb6652b9b3a0b6a91cf51ae6141291</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>REPORT_CACHE_TTL_MS</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a6e0684191970e6eba9a9f324bec14dc9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>JSON_DOC_LARGE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a256ebecb8ee9b42e4b5336937d70d5e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>TEST_SUCCESS_RATE_THRESHOLD</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a2267b26d86aa538e671357e1e8e505e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>TESTS_TOTAL_COUNT</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a52aa18e7e2370ef1f1a72569c510330b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>TESTS_PASSED_COUNT</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a1826182eacd12eb6cf57cbbb3557f121</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>TEST_SUCCESS_RATE_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a647b73cad895a836e3c5a6909cdf3462</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr const char *</type>
      <name>TEST_TIMESTAMP_EXAMPLE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a662e6893873b75bfd229f616ad336a58</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>TECH_DEBT_CODE_SMELLS</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aa4423e6784f8a6f825d820cbf347de77</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>TECH_DEBT_DUPLICATED_LINES</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>adb4dd1f340c84ec3f5127c699db14282</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>TECH_DEBT_SECURITY_HOTSPOTS</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a575275b80bfad184f03ff508871578b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>TECH_DEBT_DEBT_RATIO</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a576eb98cda5e13eb3e99fa39dc7a3757</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>TECH_DEBT_COVERAGE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a2ea2f1b7b2d3e0eb2775bbc945ec0772</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>REPORTS_AUTOREFRESH_INTERVAL_MS</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a388cf132b3a15fb4b29e070e51446295</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_SENSOR_INTERVAL_MIN_SEC</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a491767ae2faf1d45ae12ca18f5e7d810</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_SENSOR_INTERVAL_MAX_SEC</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>adf8a14dede25b2636e7aaf4608ef200b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_MQTT_INTERVAL_MIN_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aaebabdb0eb6dd0db39ae760602dee2a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_MQTT_INTERVAL_MAX_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a17326fbaaa8622ae8b100fc00fae9910</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_THINGSPEAK_INTERVAL_MIN_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ae259f192acae0a55a38efe5e452c82d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_THINGSPEAK_INTERVAL_MAX_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ad3b590a7109448376865b5364f673437</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_WEB_INTERVAL_MIN_SEC</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aa904f6ddee57ccf16b94aa9d730ea360</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_WEB_INTERVAL_MAX_SEC</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a9610ceedad0c3519ba5789d0b2bd89c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>CONFIG_DELTA_HUMIDITY_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ab26bbfd6cf3eadea4c35c0cb4c983e13</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>CONFIG_DELTA_HUMIDITY_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a0a9ab654ae2c5302b36aeb0f0608a602</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>CONFIG_DELTA_PH_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a46ccac787a6c3a6c94ab4e2fc841dc42</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>CONFIG_DELTA_PH_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a8152fc9167dfd4d69fb1cbfaa733f362</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_DELTA_EC_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a28524296b8d96ba45b453fe5d7ee0e79</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_DELTA_EC_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aa275e640d2e1cab64255f2ffc8352749</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_DELTA_NPK_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a1fd64a96986dfb606a4886cebc9198ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_DELTA_NPK_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>acfcefd90416d7b550c6e3e4382b02a05</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_AVG_WINDOW_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a899eb6ef5a8b2bd41b068cb90ba2fb38</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_AVG_WINDOW_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a0c04afa061aeca9591beecc5a6c02030</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_FORCE_CYCLES_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a57a881e326194ad08e953c16a9160a0d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>CONFIG_FORCE_CYCLES_MAX</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>af013812c414af53d50a9503b290f6aae</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>CONFIG_STEP_HUMIDITY</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a1fb2650eabd6389c2bb91cd7e9b85159</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>CONFIG_STEP_PH</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a4a9756d57a3af9c6a38767401d31db48</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>CONVERSION_SEC_TO_MS</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a8e605b770ca118fc647f51d58e88069c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>CONVERSION_MIN_TO_MS</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a67173f7d4d7cfa1392b407a9f32a7897</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr size_t</type>
      <name>CONFIG_JSON_DOC_SIZE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a5e4e19175abc22bf334b64cd325172ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr size_t</type>
      <name>CONFIG_JSON_ROOT_SIZE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a2558802bac0ba4b8f7e034b0a439964b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr size_t</type>
      <name>REPORTS_JSON_DOC_SIZE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a63009532e7b32ae5bb837e6edfe0202a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr size_t</type>
      <name>REPORTS_JSON_DOC_LARGE_SIZE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aa8dff1c70704f2253a04a57fa04d215e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr size_t</type>
      <name>OTA_JSON_DOC_SIZE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a952f6ffbe84382122e9734c64b2c2a1b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>OTA_PROGRESS_LOG_THRESHOLD</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a8a928039448dcc2c8bfe0a66c327a64c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>OTA_DELAY_MS</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>af9671ec1272b5fe627548a9a5839975c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>OTA_UPDATE_INTERVAL_MS</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aeae0cc4dca85ad5d9b5d5ac9e83c6c27</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>OTA_STAGE_CONNECTION</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a67ceb925ddd7d10a6da1b44a2785c677</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>OTA_STAGE_DOWNLOAD</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a012ba3eb455180c359435d57ebc0b205</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>OTA_STAGE_VERIFY</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a3cd517ce43b8a36499d3da2eb0b4ff29</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>OTA_STAGE_FINISH</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a64c516f5b18c9cf621e7ace9618ecfb1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>OTA_STAGE_INSTALL</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a91060f3497ce6dc7f6336ae01ce37585</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>OTA_STAGE_CHECK</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a5adc70430d136402241bec2bda3ff44f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>OTA_STAGE_DEFAULT</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ae3e0b7f10f6fe85c7cebd35434aa46e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>TEST_DATA_TEMP_BASE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a1aa343c93768dfc434a0682d1c9d0c3d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>TEST_DATA_HUM_BASE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a1528cf8eb9d4eb4d47cd3205c839ccb0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>TEST_DATA_EC_BASE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a1c85d33dd3f96b529c5d5f9a1e8403a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>TEST_DATA_PH_BASE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a811fdfb5f4a4e9c166983c8359899c08</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>TEST_DATA_NPK_BASE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ad8bee79e40c0fce273426102b809959a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>TEST_DATA_HUM_VARIATION</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ac936462f24b4986288451e14e101be82</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>TEST_DATA_EC_VARIATION</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a9737e1a41826aa52fcb468eb90a27215</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>TEST_DATA_EC_VARIATION_SMALL</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aa8041e243251863a5ae42907c403d0c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>TEST_DATA_EC_VARIATION_MIN</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a9f738c80f524f6d92bad35f523f65dc6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>TEST_DATA_NPK_FACTOR</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a5a587196d69700ba839afad383bfcf05</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>TEST_DATA_NPK_INCREASE_N</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a8294e9c0ae5f2a6632427cf046be258b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>TEST_DATA_NPK_INCREASE_P</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a46ac26f467d5f425bda128541791c48a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>TEST_DATA_NPK_INCREASE_K</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>aab269f028bf9fa04b806029f0b870331</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>TEST_DATA_NPK_DECREASE_N</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>af55b649766b90281d1f6968e22ccee01</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>TEST_DATA_NPK_DECREASE_P</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a35d3ac99cf13e2d9420e4bb7e10069f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr float</type>
      <name>TEST_DATA_NPK_DECREASE_K</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>ab45c63853fc6fec8596920c6b9fb3a76</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>HTTP_REDIRECT</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a89e8c16f10759faf32bdfae2d2644293</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr int</type>
      <name>SUCCESS_RATE_THRESHOLD</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>af594e5347d6f803166623d9db69b8664</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr unsigned long</type>
      <name>NTP_TIMESTAMP_2000</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>abbebbf1bffd3bf96d66b8d974ffd4a2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>constexpr size_t</type>
      <name>SENSOR_JSON_DOC_SIZE</name>
      <anchorfile>jxct__constants_8h.html</anchorfile>
      <anchor>a4ae108af43db0f4f03db50217d1818a6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>jxct_device_info.h</name>
    <path>include/</path>
    <filename>jxct__device__info_8h.html</filename>
    <includes id="version_8h" name="version.h" local="yes" import="no" module="no" objc="no">version.h</includes>
    <member kind="function">
      <type>String</type>
      <name>getDeviceId</name>
      <anchorfile>jxct__device__info_8h.html</anchorfile>
      <anchor>aa1f35b9688dbf1109436ad91c42901aa</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>getDefaultTopic</name>
      <anchorfile>jxct__device__info_8h.html</anchorfile>
      <anchor>a74bdf2231a5ebe05de0f3a0b4c538023</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>FIRMWARE_VERSION</name>
      <anchorfile>jxct__device__info_8h.html</anchorfile>
      <anchor>acf025b054301360475a0ec4542f74ff3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>jxct_format_utils.h</name>
    <path>include/</path>
    <filename>jxct__format__utils_8h.html</filename>
    <member kind="function">
      <type>std::string</type>
      <name>formatFloat</name>
      <anchorfile>jxct__format__utils_8h.html</anchorfile>
      <anchor>a4c645ecf7e7e9c77e89235eabf53625c</anchor>
      <arglist>(float value, int precision, bool asInt=false)</arglist>
    </member>
    <member kind="function">
      <type>std::string</type>
      <name>format_moisture</name>
      <anchorfile>jxct__format__utils_8h.html</anchorfile>
      <anchor>a3be478cef555484efe62159216f6d470</anchor>
      <arglist>(float value)</arglist>
    </member>
    <member kind="function">
      <type>std::string</type>
      <name>format_temperature</name>
      <anchorfile>jxct__format__utils_8h.html</anchorfile>
      <anchor>a9a843da2888c9b8014dd9506284f1351</anchor>
      <arglist>(float value)</arglist>
    </member>
    <member kind="function">
      <type>std::string</type>
      <name>format_ec</name>
      <anchorfile>jxct__format__utils_8h.html</anchorfile>
      <anchor>a39b2dedc3670adcf20a62a8d113d2483</anchor>
      <arglist>(float value)</arglist>
    </member>
    <member kind="function">
      <type>std::string</type>
      <name>format_ph</name>
      <anchorfile>jxct__format__utils_8h.html</anchorfile>
      <anchor>a416ab24406c338058778bbde7c4da152</anchor>
      <arglist>(float value)</arglist>
    </member>
    <member kind="function">
      <type>std::string</type>
      <name>format_npk</name>
      <anchorfile>jxct__format__utils_8h.html</anchorfile>
      <anchor>abdc9f6cdc05f0aa72eec54b3e904ca7b</anchor>
      <arglist>(float value)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>formatValue</name>
      <anchorfile>jxct__format__utils_8h.html</anchorfile>
      <anchor>ac78af75db9fe02059342eed69de396e4</anchor>
      <arglist>(float value, const char *unit, int precision=2)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>jxct_strings.h</name>
    <path>include/</path>
    <filename>jxct__strings_8h.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>API_ROOT</name>
      <anchorfile>jxct__strings_8h.html</anchorfile>
      <anchor>a2658231c913ad33de1f1e34cea47ac12</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>API_SENSOR</name>
      <anchorfile>jxct__strings_8h.html</anchorfile>
      <anchor>a47583d6152c2bf2cd0df1b41cd090268</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>API_SYSTEM</name>
      <anchorfile>jxct__strings_8h.html</anchorfile>
      <anchor>a7abdda090ad7c65c48dca0543be0b84f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>API_SYSTEM_HEALTH</name>
      <anchorfile>jxct__strings_8h.html</anchorfile>
      <anchor>a678a3a131bb0617b0db6571c255426bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>API_SYSTEM_STATUS</name>
      <anchorfile>jxct__strings_8h.html</anchorfile>
      <anchor>ab6f51e45490cf09b53508e2e28c9a52c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>API_SYSTEM_RESET</name>
      <anchorfile>jxct__strings_8h.html</anchorfile>
      <anchor>a4cdd8778645c77f73e9d718f3ab8a996</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>API_SYSTEM_REBOOT</name>
      <anchorfile>jxct__strings_8h.html</anchorfile>
      <anchor>a55dce057a184f360ba091cedc7798f5b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>API_CONFIG_EXPORT</name>
      <anchorfile>jxct__strings_8h.html</anchorfile>
      <anchor>a279a0c067a8addd61f08d5fd7993ec35</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>jxct_ui_system.h</name>
    <path>include/</path>
    <filename>jxct__ui__system_8h.html</filename>
    <class kind="struct">ButtonConfig</class>
    <member kind="define">
      <type>#define</type>
      <name>UI_COLOR_PRIMARY</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a4a027d43574b514bd2a92d66f614d641</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_COLOR_PRIMARY_HOVER</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a114e1dfb1dd3d592f05f7bad1f19dc56</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_COLOR_SECONDARY</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>ad01765df6e85bbbaadf86348f791bec8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_COLOR_SECONDARY_HOVER</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>ae215557ff301f693a631b07bb728133a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_COLOR_DANGER</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a528eb902d01c68c79d462a7b2af33253</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_COLOR_DANGER_HOVER</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a68d40d46d34e3c340432005cdcd97db9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_COLOR_WARNING</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a6b0631cd1b8e3f979781a75b72b53db3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_COLOR_SUCCESS</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>ad4197af97c0dec07ab0adecda125ec9a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_COLOR_TEXT</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>add8778175ad08e458982bb10009b8370</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_COLOR_TEXT_LIGHT</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>aae0a8f86653e473376a074dc3e0419f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_COLOR_BG</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a04a3625848045f60e1d29c0561dd683b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_COLOR_BORDER</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a332b834c8fafc656379433b5ae8977e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_CONTAINER_MAX_WIDTH</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a7af42ff3fe72e5138cce5d6fea21ee62</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_SECTION_PADDING</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a2d52deefc5b60934f93d055171ae599a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_BUTTON_PADDING</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>ab835eb27dcec3ac64adae2fbf1faba56</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_INPUT_PADDING</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>ac1518afb129e35a2eb4991ee3091e646</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_BORDER_RADIUS</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>ad15fc4d5c43f002ad00bc57f3dfd487c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_FONT_FAMILY</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a5d412f787d0afab85528cc90b78ba003</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_FONT_SIZE_BASE</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>acc76f4f1dd529ae42596cfdc0876fb55</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_FONT_SIZE_SMALL</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a20d9342cc444148594d8903ec79f055a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_FONT_SIZE_H1</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>aa9a8129049fc76d5692aa8da7f4f7074</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_FONT_SIZE_H2</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a3ea379062bdc4de13202178bda5f0e9a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_TRANSITION_FAST</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a9377f79f32bf678f828e7f7682fbeddc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_TRANSITION_NORMAL</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>aa64ee0313362e85e9592e801fac9460a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_ICON_SAVE</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>ad2ed95a952e64fc13ef8d479c8ade07f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_ICON_RESET</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>ac65cce070093672fcaf4142f9dcbe1b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_ICON_DOWNLOAD</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>aa6d659466477b3945ad48dd79bb453d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_ICON_UPLOAD</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a0de268b94c85f6f3363294f873e67096</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_ICON_CONFIG</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>ad08f44bfaadcd205bb7c3aace2131f25</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_ICON_INTERVALS</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a2855d4ff7f468f63f63d86726c7b7cce</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_ICON_DATA</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a075f0130a6ec13e19f3da60e46f893ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_ICON_SERVICE</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>aa3a7a33d5552f7ef91c086c3a1b33e52</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_ICON_WIFI</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a5c3ffccc3e511d2a1970d74ea5efd7ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_ICON_MQTT</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a230817dd54c7a5ac10a70206d7ca59f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_ICON_SUCCESS</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a5f72fda5df4f7bacdfc43cb6765b6531</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_ICON_ERROR</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a5b29a4359a5a3b032c1c554f30dad0ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_ICON_WARNING</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a3a01a904a5ea0f2e24cbb633f41b2ed9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_ICON_INFO</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>ae750f7fae3d3cceba794e364c89f9a0b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_ICON_LOCK</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a8561b079747008f35745da20f688daa6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_ICON_FOLDER</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>ab15f127830853b50a5c2f2f333794d5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_ICON_STATUS</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a5e83bf7a8415b41233dc51591952e5eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_ICON_REFRESH</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a68dbe729790d93b9811c800511aa01e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UI_ICON_CALIBRATION</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a9af26217e5d651fb664f6e3891e610b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>ButtonType</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>ade2afc06707f0601599eae163ff95153</anchor>
      <arglist></arglist>
      <enumvalue file="jxct__ui__system_8h.html" anchor="ade2afc06707f0601599eae163ff95153a428429ba9ea83e4841036fb0508fd6dc">PRIMARY</enumvalue>
      <enumvalue file="jxct__ui__system_8h.html" anchor="ade2afc06707f0601599eae163ff95153a371b09d93ca01e09a42fbd5a2a423f8e">SECONDARY</enumvalue>
      <enumvalue file="jxct__ui__system_8h.html" anchor="ade2afc06707f0601599eae163ff95153a707e893c1db5175432f341eb5d6d1ca7">DANGER</enumvalue>
      <enumvalue file="jxct__ui__system_8h.html" anchor="ade2afc06707f0601599eae163ff95153ad0749aaba8b833466dfcbb0428e4f89c">SUCCESS</enumvalue>
      <enumvalue file="jxct__ui__system_8h.html" anchor="ade2afc06707f0601599eae163ff95153a6a656275b77dd69c1cc1f49c58859151">OUTLINE</enumvalue>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>MessageType</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a3a7dcde2dab208051cb338c215f88e5e</anchor>
      <arglist></arglist>
      <enumvalue file="jxct__ui__system_8h.html" anchor="a3a7dcde2dab208051cb338c215f88e5ead0749aaba8b833466dfcbb0428e4f89c">SUCCESS</enumvalue>
      <enumvalue file="jxct__ui__system_8h.html" anchor="a3a7dcde2dab208051cb338c215f88e5eabb1ca97ec761fc37101737ba0aa2e7c5">ERROR</enumvalue>
      <enumvalue file="jxct__ui__system_8h.html" anchor="a3a7dcde2dab208051cb338c215f88e5ea059e9861e0400dfbe05c98a841f3f96b">WARNING</enumvalue>
      <enumvalue file="jxct__ui__system_8h.html" anchor="a3a7dcde2dab208051cb338c215f88e5ea551b723eafd6a31d444fcb2f5920fbd3">INFO</enumvalue>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>getUnifiedCSS</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a303ae6cee9693130ac22414e47140a1e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>getToastHTML</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>afbda6d57dd082e7767c2c5e823951472</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>getLoaderHTML</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>a3a0ab8552fa88451ee4451e40d2058b3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generateButton</name>
      <anchorfile>jxct__ui__system_8h.html</anchorfile>
      <anchor>ab544fbd8eec22a6da64c9c804981fa3e</anchor>
      <arglist>(ButtonType type, const ButtonConfig &amp;config)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>logger.h</name>
    <path>include/</path>
    <filename>logger_8h.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>LOG_SYMBOL_ERROR</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a1805ae11e986e60d931ddd9e54e12fe5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_SYMBOL_WARN</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a7e4fda439418c6c40322f686422d9780</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_SYMBOL_INFO</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a29ad49893877cde0893a8a0fef27fe90</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_SYMBOL_DEBUG</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a20a52a9bc99086288ae7984b9a4ebc2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_SYMBOL_SUCCESS</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a3f059e66d832311f77817c558143bc1f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_SYMBOL_SENSOR</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a88747f65ea7adeeda31222acace5fc11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_SYMBOL_WIFI</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a9b85a144155d4ee001653d25ce7a8ba6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_SYMBOL_MQTT</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a80f904e66b58e997255b9a283394a107</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOG_SYMBOL_HTTP</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a3b1858242a4856cb030b9b12625deb6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>COLOR_RED</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>ad86358bf19927183dd7b4ae215a29731</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>COLOR_GREEN</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>afc9149f5de51bd9ac4f5ebbfa153f018</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>COLOR_YELLOW</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a4534b577b74a58b0f4b7be027af664e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>COLOR_BLUE</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a23c70d699a5a775bc2e1ebeb8603f630</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>COLOR_MAGENTA</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a8deb0beccea721b35bdb1b4f7264fe75</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>COLOR_CYAN</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a82573859711fce56f1aa0a76b18a9b18</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>COLOR_WHITE</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a9b44987ffdc2af19b635206b94334b69</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>COLOR_RESET</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a17f760256046df23dd0ab46602f04d02</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>logPrintHeader</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>abc15635060f0257c0fffc7a81d80ba27</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>logPrintSeparator</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a3293aba3c0d14b27e1cb79043a7a46f6</anchor>
      <arglist>(symbol, length)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>logPrintBanner</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a95f2819d6c1750e96e08587db9642a63</anchor>
      <arglist>(text)</arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>LogLevel</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a4ad2f3fcf504e54f0d7835fa6a4a5cd4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LOG_ERROR</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a4ad2f3fcf504e54f0d7835fa6a4a5cd4a230506cce5c68c3bac5a821c42ed3473</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LOG_WARN</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a4ad2f3fcf504e54f0d7835fa6a4a5cd4ac8041ffa22bc823d4726701cdb13fc13</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LOG_INFO</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a4ad2f3fcf504e54f0d7835fa6a4a5cd4a6e98ff471e3ce6c4ef2d75c37ee51837</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LOG_DEBUG</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a4ad2f3fcf504e54f0d7835fa6a4a5cd4ab9f002c6ffbfd511da8090213227454e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>LogColor</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a2b15c85f545a729e30a74239be429afb</anchor>
      <arglist></arglist>
      <enumvalue file="logger_8h.html" anchor="a2b15c85f545a729e30a74239be429afbaa2d9547b5d3dd9f05984475f7c926da0">RED</enumvalue>
      <enumvalue file="logger_8h.html" anchor="a2b15c85f545a729e30a74239be429afba9de0e5dd94e861317e74964bed179fa0">GREEN</enumvalue>
      <enumvalue file="logger_8h.html" anchor="a2b15c85f545a729e30a74239be429afba8a568e5f41b7e4da88fe5c4a00aad34e">YELLOW</enumvalue>
      <enumvalue file="logger_8h.html" anchor="a2b15c85f545a729e30a74239be429afba1b3e1ee9bff86431dea6b181365ba65f">BLUE</enumvalue>
      <enumvalue file="logger_8h.html" anchor="a2b15c85f545a729e30a74239be429afbac634ffea7195608364671ac52ee59a61">MAGENTA</enumvalue>
      <enumvalue file="logger_8h.html" anchor="a2b15c85f545a729e30a74239be429afba344dd8cd533280795b9db82ef0c92749">CYAN</enumvalue>
      <enumvalue file="logger_8h.html" anchor="a2b15c85f545a729e30a74239be429afbab5bf627e448384cf3a4c35121ca6008d">WHITE</enumvalue>
      <enumvalue file="logger_8h.html" anchor="a2b15c85f545a729e30a74239be429afba2c237cb1d569e05608de5476a2f8114f">CYAN_DEFAULT</enumvalue>
    </member>
    <member kind="function">
      <type>String</type>
      <name>formatLogMessage</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a73d33bdd74467a1ac7814b1a783f7f28</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logError</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a646cbac39f467aafa298c8dae1ff0091</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logWarn</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a70f38e65c2a4620bbf763080674010af</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logInfo</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a374e6f13f686b3de469c00c01d5fe1d3</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logDebug</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>aade0576f178f655d0f991ed324fd27ff</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logSuccess</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a33d6cac9c78346491213dac197c0f9f3</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logSensor</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>ad52356734897533dd1ec07d78670a5bb</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logWiFi</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>abc98ed88066e0a808bb68d9a88ffa9c1</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logMQTT</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>ae3ae977a43dcf87df0dc3035a2983455</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logHTTP</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a8d16d7846a1d3892a73f05af6286c36e</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logSystem</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a9b60e3dbf53fc95c5a1ca77a6fec2643</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logData</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a994321edf88e16ea088cdbaf0f8a8177</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>formatLogMessageSafe</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>ac83141719078208430688a446513a86c</anchor>
      <arglist>(const char *format, Args &amp;&amp;... args)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logErrorSafe</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>ad818ba4d072fb6a9a364afa42925a3ec</anchor>
      <arglist>(const char *format, Args &amp;&amp;... args)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logWarnSafe</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>acbf7f45d915a4fb169e75a4f0a573fe1</anchor>
      <arglist>(const char *format, Args &amp;&amp;... args)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logInfoSafe</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a7aa003c7ba1e975cce3e5e166c340bcb</anchor>
      <arglist>(const char *format, Args &amp;&amp;... args)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logDebugSafe</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>af550a5f0c74d17796f43c93979519e95</anchor>
      <arglist>(const char *format, Args &amp;&amp;... args)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logSuccessSafe</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a947ab35ef1c5244ba4a67c4f816eab0f</anchor>
      <arglist>(const char *format, Args &amp;&amp;... args)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logSensorSafe</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>af76fc349877c2ee0cd1fef42c06445c5</anchor>
      <arglist>(const char *format, Args &amp;&amp;... args)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logWiFiSafe</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>af2082483c0da4d187c86ea99258a4562</anchor>
      <arglist>(const char *format, Args &amp;&amp;... args)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logMQTTSafe</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>ac46cc446803bc3bacfa163e5b7facff9</anchor>
      <arglist>(const char *format, Args &amp;&amp;... args)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logHTTPSafe</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a1f44904a4ce3eced5d95e70227139356</anchor>
      <arglist>(const char *format, Args &amp;&amp;... args)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logSystemSafe</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>ae4869858267c9cfd1b1e0bb3221825f6</anchor>
      <arglist>(const char *format, Args &amp;&amp;... args)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logDataSafe</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a606d1232524a0f9606f4ae86aedc8adf</anchor>
      <arglist>(const char *format, Args &amp;&amp;... args)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logSeparator</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a33f45b00aaf104a528bb909059246d11</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logNewline</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>ac14de3d3a99ad0c7f8ff459619f12278</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logMemoryUsage</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>ab014807afa04f9783ed19c897cbdf54d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logSystemInfo</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a36865f00b6f06ee84213f5a5a1c3c45d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logUptime</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a07e106e3f19501a64fde4b0c6fa8d73a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logWiFiStatus</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>abf70e1b094519276f0363ac763f100ab</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setLogColor</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>abbb1feb235829ffa762eb8d2615b4864</anchor>
      <arglist>(LogColor color)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resetLogColor</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>abe2743f8bba1e6d306e147b47afb0943</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>getColorCode</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>ab94ddc842b6a690863609c28cfa93590</anchor>
      <arglist>(LogColor color)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printHeader</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a490fd0c74359d15846ee34e31f95d4ae</anchor>
      <arglist>(const String &amp;title, LogColor color=LogColor::CYAN_DEFAULT)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printSubHeader</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a60b1ec225359cd3c77533138b437f600</anchor>
      <arglist>(const String &amp;title, LogColor color=LogColor::BLUE)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printTimeStamp</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>afff4c4e20076552958d756dbcdcbe366</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>LogLevel</type>
      <name>currentLogLevel</name>
      <anchorfile>logger_8h.html</anchorfile>
      <anchor>a16cca5b581e7f50a9cc5e838510ae6f5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ota_manager.h</name>
    <path>include/</path>
    <filename>ota__manager_8h.html</filename>
    <member kind="function">
      <type>void</type>
      <name>checkGuard</name>
      <anchorfile>ota__manager_8h.html</anchorfile>
      <anchor>ace424db218f3dfab484ebd7f61d44fcf</anchor>
      <arglist>(const char *tag)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>getOtaStatus</name>
      <anchorfile>ota__manager_8h.html</anchorfile>
      <anchor>a51f9f9bafeb2f56a3456ebf48a76c7b9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupOTA</name>
      <anchorfile>ota__manager_8h.html</anchorfile>
      <anchor>a8061d13b589bae87956ec09e4ea7fef5</anchor>
      <arglist>(const char *manifestUrl, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>triggerOtaCheck</name>
      <anchorfile>ota__manager_8h.html</anchorfile>
      <anchor>aab62f74691b98553d395b694e0807ae4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>triggerOtaInstall</name>
      <anchorfile>ota__manager_8h.html</anchorfile>
      <anchor>a2463eda06898e53e0ca3b3a819c995ae</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleOTA</name>
      <anchorfile>ota__manager_8h.html</anchorfile>
      <anchor>a3caeaca3fcbd288827f42df9af8b7f08</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sensor_compensation.h</name>
    <path>include/</path>
    <filename>sensor__compensation_8h.html</filename>
    <class kind="struct">EnvironmentalConditions</class>
    <class kind="struct">NPKReferences</class>
    <member kind="enumeration">
      <type></type>
      <name>SoilProfile</name>
      <anchorfile>sensor__compensation_8h.html</anchorfile>
      <anchor>aaa92ab56ea828c0bcccad4564b21155d</anchor>
      <arglist></arglist>
      <enumvalue file="sensor__compensation_8h.html" anchor="aaa92ab56ea828c0bcccad4564b21155dadd8b86e783c5b75c7c9224089b4f239b">SAND</enumvalue>
      <enumvalue file="sensor__compensation_8h.html" anchor="aaa92ab56ea828c0bcccad4564b21155da053c3b9443919054c42fe7115e111e36">LOAM</enumvalue>
      <enumvalue file="sensor__compensation_8h.html" anchor="aaa92ab56ea828c0bcccad4564b21155da2238fb7511b33ddb5f91ab723f0c3add">PEAT</enumvalue>
      <enumvalue file="sensor__compensation_8h.html" anchor="aaa92ab56ea828c0bcccad4564b21155da062285f4ae598699cec7d4b2e3a7fac2">CLAY</enumvalue>
      <enumvalue file="sensor__compensation_8h.html" anchor="aaa92ab56ea828c0bcccad4564b21155da7668b00d953ed05d4e494e67757cf0ab">SANDPEAT</enumvalue>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>SoilType</name>
      <anchorfile>sensor__compensation_8h.html</anchorfile>
      <anchor>a547d6434d73984a5c1b06e2f34a88a6e</anchor>
      <arglist></arglist>
      <enumvalue file="sensor__compensation_8h.html" anchor="a547d6434d73984a5c1b06e2f34a88a6eadd8b86e783c5b75c7c9224089b4f239b">SAND</enumvalue>
      <enumvalue file="sensor__compensation_8h.html" anchor="a547d6434d73984a5c1b06e2f34a88a6ea053c3b9443919054c42fe7115e111e36">LOAM</enumvalue>
      <enumvalue file="sensor__compensation_8h.html" anchor="a547d6434d73984a5c1b06e2f34a88a6ea2238fb7511b33ddb5f91ab723f0c3add">PEAT</enumvalue>
      <enumvalue file="sensor__compensation_8h.html" anchor="a547d6434d73984a5c1b06e2f34a88a6ea062285f4ae598699cec7d4b2e3a7fac2">CLAY</enumvalue>
      <enumvalue file="sensor__compensation_8h.html" anchor="a547d6434d73984a5c1b06e2f34a88a6ea7668b00d953ed05d4e494e67757cf0ab">SANDPEAT</enumvalue>
    </member>
    <member kind="function">
      <type>float</type>
      <name>correctEC</name>
      <anchorfile>sensor__compensation_8h.html</anchorfile>
      <anchor>af586e2e5563baee2b53964aff0e1467d</anchor>
      <arglist>(float ecRaw, float T, float theta, SoilType soil)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>correctPH</name>
      <anchorfile>sensor__compensation_8h.html</anchorfile>
      <anchor>ac0fa58b3f7b7eda140b84c054a62db71</anchor>
      <arglist>(float T, float phRaw)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>correctNPK</name>
      <anchorfile>sensor__compensation_8h.html</anchorfile>
      <anchor>a183d2dfc2de22145300e91ce25f93339</anchor>
      <arglist>(float T, float theta, SoilType soil, NPKReferences &amp;npk)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>correctEC</name>
      <anchorfile>sensor__compensation_8h.html</anchorfile>
      <anchor>afafc0921b1ae4ac19063de1828f7ac4b</anchor>
      <arglist>(float ecRaw, const EnvironmentalConditions &amp;env, SoilType soil)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>correctNPK</name>
      <anchorfile>sensor__compensation_8h.html</anchorfile>
      <anchor>a9406fc6bd5dad362d51fcf4547cd279a</anchor>
      <arglist>(const EnvironmentalConditions &amp;env, NPKReferences &amp;npk, SoilType soil)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sensor_factory.h</name>
    <path>include/</path>
    <filename>sensor__factory_8h.html</filename>
    <includes id="ISensor_8h" name="ISensor.h" local="yes" import="no" module="no" objc="no">ISensor.h</includes>
    <includes id="fake__sensor__adapter_8h" name="fake_sensor_adapter.h" local="yes" import="no" module="no" objc="no">fake_sensor_adapter.h</includes>
    <includes id="jxct__config__vars_8h" name="jxct_config_vars.h" local="yes" import="no" module="no" objc="no">jxct_config_vars.h</includes>
    <includes id="modbus__sensor__adapter_8h" name="modbus_sensor_adapter.h" local="yes" import="no" module="no" objc="no">modbus_sensor_adapter.h</includes>
    <member kind="function" static="yes">
      <type>static std::unique_ptr&lt; ISensor &gt;</type>
      <name>createSensorInstance</name>
      <anchorfile>sensor__factory_8h.html</anchorfile>
      <anchor>a1a894ffc1fd37d020b3e91a9fe65fcab</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>validation_utils.h</name>
    <path>include/</path>
    <filename>validation__utils_8h.html</filename>
    <includes id="modbus__sensor_8h" name="modbus_sensor.h" local="yes" import="no" module="no" objc="no">modbus_sensor.h</includes>
    <class kind="struct">ValidationResult</class>
    <class kind="struct">ValidationError</class>
    <class kind="struct">ConfigValidationResult</class>
    <class kind="struct">SensorValidationResult</class>
    <class kind="struct">ConfigData</class>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateSSID</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>af988ebb1f8e6e907df08f24a81d34c6b</anchor>
      <arglist>(const String &amp;ssid)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validatePassword</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>a9e87dd09b18043e14aeaf09bb1ed4577</anchor>
      <arglist>(const String &amp;password)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateMQTTServer</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>aa496a6a13d80b8662e7dfea4fd9df479</anchor>
      <arglist>(const String &amp;server)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateMQTTPort</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>a47bd7b80bf2f4f9b117609e7e1cc507c</anchor>
      <arglist>(int port)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateThingSpeakAPIKey</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>aed42aace8ccac73c73eb1440f8cd2afc</anchor>
      <arglist>(const String &amp;apiKey)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateInterval</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>a497c34deeef98edaac0de3ac1a3e10a8</anchor>
      <arglist>(unsigned long interval, unsigned long min_val, unsigned long max_val, const char *name)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateSensorReadInterval</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>a7f07d4d6084e7aa1d2de3a8b5661d9e3</anchor>
      <arglist>(unsigned long interval)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateMQTTPublishInterval</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>a15f556efd2c016821186e4604e1d94a5</anchor>
      <arglist>(unsigned long interval)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateThingSpeakInterval</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>a5b75ecc2060e8847777ac37a7b17baf2</anchor>
      <arglist>(unsigned long interval)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateNTPInterval</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>affc3bd3259b06ba5eaaec72c6196886d</anchor>
      <arglist>(unsigned long interval)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateRange</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>aee1ec6b01bc6185c471610c55ff89fb2</anchor>
      <arglist>(float value, float min_val, float max_val, const char *field_name)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateTemperature</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>afe3a5befa4789b9ca6c7655d6281f888</anchor>
      <arglist>(float temperature)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateHumidity</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>a704129fdab28312c377892a04c574a4b</anchor>
      <arglist>(float humidity)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validatePH</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>a9a22805be7f816c485280fdae9ad0fd9</anchor>
      <arglist>(float ph)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateEC</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>ad576be2a825399670a0d7ad52dfc8425</anchor>
      <arglist>(float ec)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateNPK</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>a60ce7cf3c6ae19175789b058eaaede80</anchor>
      <arglist>(float value, const char *nutrient)</arglist>
    </member>
    <member kind="function">
      <type>ConfigValidationResult</type>
      <name>validateFullConfig</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>a50082e2c8fdc2515c34d4c5a38651242</anchor>
      <arglist>(const ConfigData &amp;config, bool checkRequired=true)</arglist>
    </member>
    <member kind="function">
      <type>SensorValidationResult</type>
      <name>validateFullSensorData</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>a1b8529ff5465c73b043d27ad7d2e86f3</anchor>
      <arglist>(const SensorData &amp;data)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isValidIPAddress</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>a15290a120cd4c378e64893f11adbb4fd</anchor>
      <arglist>(const String &amp;ip)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isValidHostname</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>abf0e43fe7c5fd9b3439d01e4a0557259</anchor>
      <arglist>(const String &amp;hostname)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>formatValidationErrors</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>af447043f0553ccee92c72fb59009b1ec</anchor>
      <arglist>(const ConfigValidationResult &amp;result)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>formatSensorValidationErrors</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>aecb050d7cbd49c3a3cb0dadcb72360c3</anchor>
      <arglist>(const SensorValidationResult &amp;result)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logValidationResult</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>a5b0b5f3adf97cc7910641d046b3e292c</anchor>
      <arglist>(const ConfigValidationResult &amp;result, const char *context)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logSensorValidationResult</name>
      <anchorfile>validation__utils_8h.html</anchorfile>
      <anchor>a90392289dbf6077016cdb0ce0cdc34f8</anchor>
      <arglist>(const SensorValidationResult &amp;result, const char *context)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>version.h</name>
    <path>include/</path>
    <filename>version_8h.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>JXCT_VERSION_MAJOR</name>
      <anchorfile>version_8h.html</anchorfile>
      <anchor>a6a112058ddea198337c7a135120d6b21</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>JXCT_VERSION_MINOR</name>
      <anchorfile>version_8h.html</anchorfile>
      <anchor>a6ff229495fbd3dddb40a545ea08dff83</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>JXCT_VERSION_PATCH</name>
      <anchorfile>version_8h.html</anchorfile>
      <anchor>ae65128d504d49e3657fdbedeea27f3de</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STRINGIFY</name>
      <anchorfile>version_8h.html</anchorfile>
      <anchor>a6df1d22fb5f09eccc23b9f399670cfd7</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TOSTRING</name>
      <anchorfile>version_8h.html</anchorfile>
      <anchor>a9063e80f8777300c93afde6e6f4c9cea</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>JXCT_VERSION_STRING</name>
      <anchorfile>version_8h.html</anchorfile>
      <anchor>aba3d86f9adebd869f9dabcde75a28eab</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>JXCT_VERSION_CODE</name>
      <anchorfile>version_8h.html</anchorfile>
      <anchor>abfca905ae033e33cf0310e0d283552e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>JXCT_VERSION_AT_LEAST</name>
      <anchorfile>version_8h.html</anchorfile>
      <anchor>a3500b43e0da5d3814772835fa8f92963</anchor>
      <arglist>(major, minor, patch)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>JXCT_BUILD_DATE</name>
      <anchorfile>version_8h.html</anchorfile>
      <anchor>a54c102c9bd71e3382ce683efe7b3407b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>JXCT_BUILD_TIME</name>
      <anchorfile>version_8h.html</anchorfile>
      <anchor>aa02b5d2d03e3dbdfc2a4a0bcc8ce2818</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>JXCT_FULL_VERSION_STRING</name>
      <anchorfile>version_8h.html</anchorfile>
      <anchor>a0096f2930a233d1d076483aa61c5b94f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char</type>
      <name>DEVICE_MANUFACTURER</name>
      <anchorfile>version_8h.html</anchorfile>
      <anchor>a83f77c2317b3d28560f91fffca64efea</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char</type>
      <name>DEVICE_MODEL</name>
      <anchorfile>version_8h.html</anchorfile>
      <anchor>acd8d2dccf471a5d0cbd070ff1bef8887</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char</type>
      <name>DEVICE_SW_VERSION</name>
      <anchorfile>version_8h.html</anchorfile>
      <anchor>aa6399036840a604f6b3bbbf93f7ecd3c</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const char *</type>
      <name>FIRMWARE_VERSION</name>
      <anchorfile>version_8h.html</anchorfile>
      <anchor>acf025b054301360475a0ec4542f74ff3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>csrf_protection.h</name>
    <path>include/web/</path>
    <filename>csrf__protection_8h.html</filename>
    <member kind="function">
      <type>String</type>
      <name>generateCSRFToken</name>
      <anchorfile>csrf__protection_8h.html</anchorfile>
      <anchor>ab27a975e89338b0b23a40e028b1ae9b2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>validateCSRFToken</name>
      <anchorfile>csrf__protection_8h.html</anchorfile>
      <anchor>a936ecd18be81531b306c2c4705f7e860</anchor>
      <arglist>(const String &amp;token)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>getCSRFHiddenField</name>
      <anchorfile>csrf__protection_8h.html</anchorfile>
      <anchor>aab8be89322026594573b8f3c8064bbbc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>checkCSRFSafety</name>
      <anchorfile>csrf__protection_8h.html</anchorfile>
      <anchor>a0320c55f4de318df92de8a73e8e7d828</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initCSRFProtection</name>
      <anchorfile>csrf__protection_8h.html</anchorfile>
      <anchor>ac922ac6444b8b990e13e3746ca931065</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>methodToString</name>
      <anchorfile>csrf__protection_8h.html</anchorfile>
      <anchor>ad9d33eff9bf83f09111cc522049930bc</anchor>
      <arglist>(HTTPMethod method)</arglist>
    </member>
    <member kind="variable">
      <type>WebServer</type>
      <name>webServer</name>
      <anchorfile>csrf__protection_8h.html</anchorfile>
      <anchor>a6385fd6a6118223757bf00a8ba828562</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>web_routes.h</name>
    <path>include/</path>
    <filename>web__routes_8h.html</filename>
    <includes id="wifi__manager_8h" name="wifi_manager.h" local="yes" import="no" module="no" objc="no">../src/wifi_manager.h</includes>
    <includes id="jxct__strings_8h" name="jxct_strings.h" local="yes" import="no" module="no" objc="no">jxct_strings.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">logger.h</includes>
    <member kind="function">
      <type>String</type>
      <name>generateCSRFToken</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>ab27a975e89338b0b23a40e028b1ae9b2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>validateCSRFToken</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a936ecd18be81531b306c2c4705f7e860</anchor>
      <arglist>(const String &amp;token)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>getCSRFHiddenField</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>aab8be89322026594573b8f3c8064bbbc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>checkCSRFSafety</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a0320c55f4de318df92de8a73e8e7d828</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initCSRFProtection</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>ac922ac6444b8b990e13e3746ca931065</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>methodToString</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>ad9d33eff9bf83f09111cc522049930bc</anchor>
      <arglist>(HTTPMethod method)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupMainRoutes</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a6a7cf6b6ba01d34721e1a47ea65af867</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleRoot</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a8427468f39342a1990703e9f5ce7fd29</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleStatus</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a1e811785ccbaccda2e3e3457971720cb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupConfigRoutes</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a0e7c19a3399afd6ceae18c4512cca24c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleIntervals</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a66a3c3cf6ad057d6d3dbceda7f120a73</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleAdvanced</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a2cb21fe9b6b83094c18247812977bb85</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleApiConfigImport</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a806651e5fe403b18cc043dfdaf34ab12</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleApiConfigExport</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>ae13d89074e240a307ef4ad70c6acd5f0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupServiceRoutes</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a32c74f47a23de058ee90a43af2dff066</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleReset</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a28576d900afa7ce8f3f4177cb38f61b9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleInfo</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>ad77ad5405e56bcd9287e86ed07e065c3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleUpdateStatus</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>aca63e6b9c5fd7f5d563797216fb07b7c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupDataRoutes</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>aed1136f683a386e56678e05fc3747972</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sendSensorJson</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a636822ce9463007c8c3e5b10d00b26e1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleReadings</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a8d9f48b293b7ec9974c1d7241436529b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleReadingsUpload</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a43999630c470d7068023c4f423c0722c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isRouteAvailable</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a195f4874889e332e6f540a518d6c51fe</anchor>
      <arglist>(const String &amp;uri)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isFeatureAvailable</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>aef0848857422724f7d825dd8a33421b9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logWebRequest</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a1a62e7c40bff885d97f6018743bec642</anchor>
      <arglist>(const String &amp;method, const String &amp;uri, const String &amp;clientIP)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generateForm</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>ad6e617b3350bd617099ba38b52cd3076</anchor>
      <arglist>(const String &amp;action, const String &amp;method, const String &amp;formContent, const String &amp;buttonText, const String &amp;buttonIcon)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generateConfigSection</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a6ea1f07e47da42a04aa3dbf1829ceb61</anchor>
      <arglist>(const String &amp;title, const String &amp;content, const String &amp;helpText=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generateInputField</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>aa3bf8c4a6f5d5755121de67cf17ff422</anchor>
      <arglist>(const String &amp;id, const String &amp;name, const String &amp;label, const String &amp;value, const String &amp;type=&quot;text&quot;, bool required=false, const String &amp;placeholder=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generateCheckboxField</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>ae920cb227a775afac1415bab5339c623</anchor>
      <arglist>(const String &amp;id, const String &amp;name, const String &amp;label, bool checked)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generateNumberField</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a8588b654fd55604bdb4fc112191c195f</anchor>
      <arglist>(const String &amp;id, const String &amp;name, const String &amp;label, int value, int min, int max, int step=1)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generateFormError</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>af68ca26d269d5d8ea7fa42fc78a5a919</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupErrorHandlers</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a833be2e68cb85aa95168dae9c9b128ef</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>validateConfigInput</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>ad738b39272f7fc383f5bb83814fb1a67</anchor>
      <arglist>(bool checkRequired=true)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleUploadError</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a72ddd36cf01d539c0b2575750fa997dc</anchor>
      <arglist>(const String &amp;error)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generatePageHeader</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>ad0518ce62fefc0ff55e7cd72e75db70d</anchor>
      <arglist>(const String &amp;title, const String &amp;icon=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generatePageFooter</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>ad4f3a6e85baef9b2d3e2aec977c3a321</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generateErrorPage</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a104303f7b44cf43e481a713261d52457</anchor>
      <arglist>(int errorCode, const String &amp;errorMessage)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generateSuccessPage</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>acc502267a202aba9171c0d593fbfac17</anchor>
      <arglist>(const String &amp;title, const String &amp;message, const String &amp;redirectUrl=&quot;&quot;, int redirectDelay=2)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generateBasePage</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a03709c52c03116ff221a501c3f0c69b2</anchor>
      <arglist>(const String &amp;title, const String &amp;content, const String &amp;icon=&quot;&quot;)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generateValidationErrorResponse</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a6fe24c5dc21b012791aecd16ab4dad96</anchor>
      <arglist>(const String &amp;errorMsg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleCriticalError</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a200ff078a206ee5a0b44bc0c5411f3f5</anchor>
      <arglist>(const String &amp;error)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generateApModeUnavailablePage</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a1d57b5edf50f600cc5d02641cb4510a0</anchor>
      <arglist>(const String &amp;title, const String &amp;icon)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupOtaRoutes</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>ae58a54419ee068a06deeb05bf1e771f4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupReportsRoutes</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a1330f997c3d2a043a5ac4efc8f9906e7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>WebServer</type>
      <name>webServer</name>
      <anchorfile>web__routes_8h.html</anchorfile>
      <anchor>a6385fd6a6118223757bf00a8ba828562</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>advanced_filters.cpp</name>
    <path>src/</path>
    <filename>advanced__filters_8cpp.html</filename>
    <includes id="advanced__filters_8h" name="advanced_filters.h" local="yes" import="no" module="no" objc="no">advanced_filters.h</includes>
    <includes id="jxct__config__vars_8h" name="jxct_config_vars.h" local="yes" import="no" module="no" objc="no">jxct_config_vars.h</includes>
    <includes id="jxct__constants_8h" name="jxct_constants.h" local="yes" import="no" module="no" objc="no">jxct_constants.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">logger.h</includes>
    <includes id="modbus__sensor_8h" name="modbus_sensor.h" local="yes" import="no" module="no" objc="no">modbus_sensor.h</includes>
    <class kind="struct">AdvancedFilters::ExponentialSmoothingState</class>
    <class kind="struct">AdvancedFilters::StatisticsBuffer</class>
    <class kind="struct">AdvancedFilters::KalmanFilter</class>
    <class kind="struct">AdvancedFilters::ECFilterState</class>
    <namespace>AdvancedFilters</namespace>
  </compound>
  <compound kind="file">
    <name>crop_recommendation_engine.cpp</name>
    <path>src/business/</path>
    <filename>crop__recommendation__engine_8cpp.html</filename>
    <includes id="crop__recommendation__engine_8h" name="crop_recommendation_engine.h" local="yes" import="no" module="no" objc="no">crop_recommendation_engine.h</includes>
    <includes id="jxct__config__vars_8h" name="jxct_config_vars.h" local="yes" import="no" module="no" objc="no">../../include/jxct_config_vars.h</includes>
    <includes id="jxct__constants_8h" name="jxct_constants.h" local="yes" import="no" module="no" objc="no">../../include/jxct_constants.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">../../include/logger.h</includes>
    <includes id="validation__utils_8h" name="validation_utils.h" local="yes" import="no" module="no" objc="no">validation_utils.h</includes>
    <class kind="struct">RecommendationParams</class>
    <class kind="struct">RecommendationParams::Builder</class>
    <member kind="function">
      <type>float</type>
      <name>compensatePHInternal</name>
      <anchorfile>crop__recommendation__engine_8cpp.html</anchorfile>
      <anchor>a6875f9609c03530614608cf63548b19d</anchor>
      <arglist>(float pHRawValue, float temperatureValue, float moistureValue)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>compensateECInternal</name>
      <anchorfile>crop__recommendation__engine_8cpp.html</anchorfile>
      <anchor>a65ee2eb89935cdb909da750ca153c2e7</anchor>
      <arglist>(float ECRawValue, float temperatureValue)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>compensateNPKInternal</name>
      <anchorfile>crop__recommendation__engine_8cpp.html</anchorfile>
      <anchor>a2df14d4854b2ddc0f5c61bd9372a984e</anchor>
      <arglist>(float NPKRawValue, float temperatureValue, float moistureValue)</arglist>
    </member>
    <member kind="function">
      <type>CropConfig</type>
      <name>applySeasonalAdjustments</name>
      <anchorfile>crop__recommendation__engine_8cpp.html</anchorfile>
      <anchor>a6d9f608ac1a54cf50c6962d1fd9e643f</anchor>
      <arglist>(const CropConfig &amp;base, const String &amp;season)</arglist>
    </member>
    <member kind="function">
      <type>CropConfig</type>
      <name>applyGrowingTypeAdjustments</name>
      <anchorfile>crop__recommendation__engine_8cpp.html</anchorfile>
      <anchor>a90e7bb05ccb5ef0c224050c22ddf7330</anchor>
      <arglist>(const CropConfig &amp;base, const String &amp;growingType)</arglist>
    </member>
    <member kind="function">
      <type>CropConfig</type>
      <name>applySoilTypeAdjustments</name>
      <anchorfile>crop__recommendation__engine_8cpp.html</anchorfile>
      <anchor>ad389b9a72db017bea3289440e818dec5</anchor>
      <arglist>(const CropConfig &amp;base, const String &amp;soilType)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>crop_recommendation_engine.h</name>
    <path>src/business/</path>
    <filename>crop__recommendation__engine_8h.html</filename>
    <includes id="ICropRecommendationEngine_8h" name="ICropRecommendationEngine.h" local="yes" import="no" module="no" objc="no">business/ICropRecommendationEngine.h</includes>
    <includes id="modbus__sensor_8h" name="modbus_sensor.h" local="yes" import="no" module="no" objc="no">../modbus_sensor.h</includes>
    <class kind="struct">CropCompensationParams</class>
    <class kind="struct">CropCompensationParams::Builder</class>
    <class kind="struct">CropECCompensationParams</class>
    <class kind="struct">CropECCompensationParams::Builder</class>
    <class kind="struct">CropConfig</class>
    <class kind="struct">RecommendationResult</class>
    <class kind="class">CropRecommendationEngine</class>
  </compound>
  <compound kind="file">
    <name>sensor_calibration_service.cpp</name>
    <path>src/business/</path>
    <filename>sensor__calibration__service_8cpp.html</filename>
    <includes id="src_2business_2sensor__calibration__service_8h" name="sensor_calibration_service.h" local="yes" import="no" module="no" objc="no">sensor_calibration_service.h</includes>
    <includes id="include_2calibration__manager_8h" name="calibration_manager.h" local="yes" import="no" module="no" objc="no">../../include/calibration_manager.h</includes>
    <includes id="jxct__constants_8h" name="jxct_constants.h" local="yes" import="no" module="no" objc="no">../../include/jxct_constants.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">../../include/logger.h</includes>
    <includes id="validation__utils_8h" name="validation_utils.h" local="yes" import="no" module="no" objc="no">validation_utils.h</includes>
    <member kind="function">
      <type>std::map&lt; SoilProfile, CalibrationTable &gt; &amp;</type>
      <name>getCalibrationTables</name>
      <anchorfile>sensor__calibration__service_8cpp.html</anchorfile>
      <anchor>abab1dae6416ffacbbb2b44dbc79938f6</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sensor_calibration_service.h</name>
    <path>include/</path>
    <filename>include_2sensor__calibration__service_8h.html</filename>
    <class kind="struct">CalibrationPoint</class>
    <class kind="struct">PHCalibration</class>
    <class kind="struct">ECCalibration</class>
    <class kind="struct">NPKCalibration</class>
    <class kind="struct">SensorCalibration</class>
    <class kind="class">SensorCalibrationService</class>
  </compound>
  <compound kind="file">
    <name>sensor_calibration_service.h</name>
    <path>src/business/</path>
    <filename>src_2business_2sensor__calibration__service_8h.html</filename>
    <includes id="ISensorCalibrationService_8h" name="ISensorCalibrationService.h" local="yes" import="no" module="no" objc="no">../../include/business/ISensorCalibrationService.h</includes>
    <includes id="include_2calibration__manager_8h" name="calibration_manager.h" local="yes" import="no" module="no" objc="no">../../include/calibration_manager.h</includes>
    <includes id="sensor__compensation_8h" name="sensor_compensation.h" local="yes" import="no" module="no" objc="no">../../include/sensor_compensation.h</includes>
    <includes id="validation__utils_8h" name="validation_utils.h" local="yes" import="no" module="no" objc="no">../../include/validation_utils.h</includes>
    <class kind="struct">CalibrationPoint</class>
    <class kind="struct">CalibrationTable</class>
    <class kind="class">SensorCalibrationService</class>
  </compound>
  <compound kind="file">
    <name>sensor_compensation_service.cpp</name>
    <path>src/business/</path>
    <filename>sensor__compensation__service_8cpp.html</filename>
    <includes id="sensor__compensation__service_8h" name="sensor_compensation_service.h" local="yes" import="no" module="no" objc="no">sensor_compensation_service.h</includes>
    <includes id="jxct__constants_8h" name="jxct_constants.h" local="yes" import="no" module="no" objc="no">../../include/jxct_constants.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">../../include/logger.h</includes>
    <includes id="sensor__compensation_8h" name="sensor_compensation.h" local="yes" import="no" module="no" objc="no">../../include/sensor_compensation.h</includes>
  </compound>
  <compound kind="file">
    <name>sensor_compensation_service.h</name>
    <path>src/business/</path>
    <filename>sensor__compensation__service_8h.html</filename>
    <includes id="ISensorCompensationService_8h" name="ISensorCompensationService.h" local="yes" import="no" module="no" objc="no">../../include/business/ISensorCompensationService.h</includes>
    <includes id="sensor__compensation_8h" name="sensor_compensation.h" local="yes" import="no" module="no" objc="no">../../include/sensor_compensation.h</includes>
    <includes id="validation__utils_8h" name="validation_utils.h" local="yes" import="no" module="no" objc="no">../../include/validation_utils.h</includes>
    <class kind="struct">ArchieCoefficients</class>
    <class kind="struct">SoilParameters</class>
    <class kind="struct">NPKCoefficients</class>
    <class kind="class">SensorCompensationService</class>
  </compound>
  <compound kind="file">
    <name>business_instances.cpp</name>
    <path>src/</path>
    <filename>business__instances_8cpp.html</filename>
    <includes id="crop__recommendation__engine_8h" name="crop_recommendation_engine.h" local="yes" import="no" module="no" objc="no">business/crop_recommendation_engine.h</includes>
    <includes id="src_2business_2sensor__calibration__service_8h" name="sensor_calibration_service.h" local="yes" import="no" module="no" objc="no">business/sensor_calibration_service.h</includes>
    <includes id="sensor__compensation__service_8h" name="sensor_compensation_service.h" local="yes" import="no" module="no" objc="no">business/sensor_compensation_service.h</includes>
    <member kind="variable">
      <type>CropRecommendationEngine</type>
      <name>gCropEngine</name>
      <anchorfile>business__instances_8cpp.html</anchorfile>
      <anchor>a846779af5e07f808b2cee49916205a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SensorCalibrationService</type>
      <name>gCalibrationService</name>
      <anchorfile>business__instances_8cpp.html</anchorfile>
      <anchor>a2048f65bc87ffffe1cdadbacc0e95a58</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SensorCompensationService</type>
      <name>gCompensationService</name>
      <anchorfile>business__instances_8cpp.html</anchorfile>
      <anchor>a18619e11a2ee0b61db56d4fd39fbc6c3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>business_services.cpp</name>
    <path>src/</path>
    <filename>business__services_8cpp.html</filename>
    <includes id="business__services_8h" name="business_services.h" local="yes" import="no" module="no" objc="no">business_services.h</includes>
    <includes id="crop__recommendation__engine_8h" name="crop_recommendation_engine.h" local="yes" import="no" module="no" objc="no">business/crop_recommendation_engine.h</includes>
    <includes id="src_2business_2sensor__calibration__service_8h" name="sensor_calibration_service.h" local="yes" import="no" module="no" objc="no">business/sensor_calibration_service.h</includes>
    <includes id="sensor__compensation__service_8h" name="sensor_compensation_service.h" local="yes" import="no" module="no" objc="no">business/sensor_compensation_service.h</includes>
    <member kind="function">
      <type>ICropRecommendationEngine &amp;</type>
      <name>getCropEngine</name>
      <anchorfile>business__services_8cpp.html</anchorfile>
      <anchor>a9e42803bf759fbcfcce0c55a600f6590</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>ISensorCalibrationService &amp;</type>
      <name>getCalibrationService</name>
      <anchorfile>business__services_8cpp.html</anchorfile>
      <anchor>af5557d1c7a1e7954fd87d6df62d881c9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>ISensorCompensationService &amp;</type>
      <name>getCompensationService</name>
      <anchorfile>business__services_8cpp.html</anchorfile>
      <anchor>a534645a7c06e83e207c21a8c48924248</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>CropRecommendationEngine</type>
      <name>gCropEngine</name>
      <anchorfile>business__services_8cpp.html</anchorfile>
      <anchor>a846779af5e07f808b2cee49916205a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SensorCalibrationService</type>
      <name>gCalibrationService</name>
      <anchorfile>business__services_8cpp.html</anchorfile>
      <anchor>a2048f65bc87ffffe1cdadbacc0e95a58</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SensorCompensationService</type>
      <name>gCompensationService</name>
      <anchorfile>business__services_8cpp.html</anchorfile>
      <anchor>a18619e11a2ee0b61db56d4fd39fbc6c3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>calibration_manager.cpp</name>
    <path>src/</path>
    <filename>calibration__manager_8cpp.html</filename>
    <includes id="src_2calibration__manager_8h" name="calibration_manager.h" local="yes" import="no" module="no" objc="no">calibration_manager.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">logger.h</includes>
    <includes id="sensor__compensation_8h" name="sensor_compensation.h" local="yes" import="no" module="no" objc="no">sensor_compensation.h</includes>
    <namespace>CalibrationManager</namespace>
  </compound>
  <compound kind="file">
    <name>calibration_manager.h</name>
    <path>include/</path>
    <filename>include_2calibration__manager_8h.html</filename>
    <includes id="sensor__compensation_8h" name="sensor_compensation.h" local="yes" import="no" module="no" objc="no">sensor_compensation.h</includes>
    <class kind="struct">CalibrationEntry</class>
    <namespace>CalibrationManager</namespace>
  </compound>
  <compound kind="file">
    <name>calibration_manager.h</name>
    <path>src/</path>
    <filename>src_2calibration__manager_8h.html</filename>
    <includes id="sensor__compensation_8h" name="sensor_compensation.h" local="yes" import="no" module="no" objc="no">sensor_compensation.h</includes>
    <class kind="struct">CalibrationEntry</class>
    <namespace>CalibrationManager</namespace>
  </compound>
  <compound kind="file">
    <name>config.cpp</name>
    <path>src/</path>
    <filename>config_8cpp.html</filename>
    <includes id="debug_8h" name="debug.h" local="yes" import="no" module="no" objc="no">debug.h</includes>
    <includes id="jxct__config__vars_8h" name="jxct_config_vars.h" local="yes" import="no" module="no" objc="no">jxct_config_vars.h</includes>
    <includes id="jxct__constants_8h" name="jxct_constants.h" local="yes" import="no" module="no" objc="no">jxct_constants.h</includes>
    <includes id="jxct__device__info_8h" name="jxct_device_info.h" local="yes" import="no" module="no" objc="no">jxct_device_info.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">logger.h</includes>
    <includes id="version_8h" name="version.h" local="yes" import="no" module="no" objc="no">version.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>KEY_NTP_INTERVAL</name>
      <anchorfile>config_8cpp.html</anchorfile>
      <anchor>a9ccd2b50755f4e9983b7f4773a19469f</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>getDeviceId</name>
      <anchorfile>config_8cpp.html</anchorfile>
      <anchor>aa1f35b9688dbf1109436ad91c42901aa</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>getDefaultTopic</name>
      <anchorfile>config_8cpp.html</anchorfile>
      <anchor>a74bdf2231a5ebe05de0f3a0b4c538023</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>loadConfig</name>
      <anchorfile>config_8cpp.html</anchorfile>
      <anchor>ad5ed6ddd9940c0097cc91774056df1c2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>saveConfig</name>
      <anchorfile>config_8cpp.html</anchorfile>
      <anchor>a688d00bbabd28fbaf9e0c50eca3adeae</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resetConfig</name>
      <anchorfile>config_8cpp.html</anchorfile>
      <anchor>a6fa5204bcafe1db397da4a87b1690061</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isConfigValid</name>
      <anchorfile>config_8cpp.html</anchorfile>
      <anchor>aeb61f04cd3b4b68e8146d85c447d70d1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>Config</type>
      <name>config</name>
      <anchorfile>config_8cpp.html</anchorfile>
      <anchor>a4a8dd3a2de125b72d4fe6251a0a271b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>Preferences</type>
      <name>preferences</name>
      <anchorfile>config_8cpp.html</anchorfile>
      <anchor>a6ea06cf7b8092a0adaf07614d7ece59d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>fake_sensor.cpp</name>
    <path>src/</path>
    <filename>fake__sensor_8cpp.html</filename>
    <includes id="fake__sensor_8h" name="fake_sensor.h" local="yes" import="no" module="no" objc="no">fake_sensor.h</includes>
    <includes id="debug_8h" name="debug.h" local="yes" import="no" module="no" objc="no">debug.h</includes>
    <includes id="jxct__config__vars_8h" name="jxct_config_vars.h" local="yes" import="no" module="no" objc="no">jxct_config_vars.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">logger.h</includes>
    <includes id="modbus__sensor_8h" name="modbus_sensor.h" local="yes" import="no" module="no" objc="no">modbus_sensor.h</includes>
    <includes id="sensor__compensation_8h" name="sensor_compensation.h" local="yes" import="no" module="no" objc="no">sensor_compensation.h</includes>
    <member kind="function">
      <type>void</type>
      <name>startFakeSensorTask</name>
      <anchorfile>fake__sensor_8cpp.html</anchorfile>
      <anchor>abd277e18147efb1103e91eac01627612</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>fake_sensor.h</name>
    <path>src/</path>
    <filename>fake__sensor_8h.html</filename>
    <member kind="function">
      <type>void</type>
      <name>startFakeSensorTask</name>
      <anchorfile>fake__sensor_8h.html</anchorfile>
      <anchor>abd277e18147efb1103e91eac01627612</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>fake_sensor_adapter.h</name>
    <path>src/</path>
    <filename>fake__sensor__adapter_8h.html</filename>
    <includes id="basic__sensor__adapter_8h" name="basic_sensor_adapter.h" local="yes" import="no" module="no" objc="no">basic_sensor_adapter.h</includes>
    <includes id="fake__sensor_8h" name="fake_sensor.h" local="yes" import="no" module="no" objc="no">fake_sensor.h</includes>
    <includes id="modbus__sensor_8h" name="modbus_sensor.h" local="yes" import="no" module="no" objc="no">modbus_sensor.h</includes>
    <class kind="class">FakeSensorAdapter</class>
  </compound>
  <compound kind="file">
    <name>jxct_format_utils.cpp</name>
    <path>src/</path>
    <filename>jxct__format__utils_8cpp.html</filename>
    <includes id="jxct__format__utils_8h" name="jxct_format_utils.h" local="yes" import="no" module="no" objc="no">jxct_format_utils.h</includes>
    <member kind="function">
      <type>std::string</type>
      <name>format_moisture</name>
      <anchorfile>jxct__format__utils_8cpp.html</anchorfile>
      <anchor>a3be478cef555484efe62159216f6d470</anchor>
      <arglist>(float value)</arglist>
    </member>
    <member kind="function">
      <type>std::string</type>
      <name>format_temperature</name>
      <anchorfile>jxct__format__utils_8cpp.html</anchorfile>
      <anchor>a9a843da2888c9b8014dd9506284f1351</anchor>
      <arglist>(float value)</arglist>
    </member>
    <member kind="function">
      <type>std::string</type>
      <name>format_ec</name>
      <anchorfile>jxct__format__utils_8cpp.html</anchorfile>
      <anchor>a39b2dedc3670adcf20a62a8d113d2483</anchor>
      <arglist>(float value)</arglist>
    </member>
    <member kind="function">
      <type>std::string</type>
      <name>format_ph</name>
      <anchorfile>jxct__format__utils_8cpp.html</anchorfile>
      <anchor>a416ab24406c338058778bbde7c4da152</anchor>
      <arglist>(float value)</arglist>
    </member>
    <member kind="function">
      <type>std::string</type>
      <name>format_npk</name>
      <anchorfile>jxct__format__utils_8cpp.html</anchorfile>
      <anchor>abdc9f6cdc05f0aa72eec54b3e904ca7b</anchor>
      <arglist>(float value)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>formatValue</name>
      <anchorfile>jxct__format__utils_8cpp.html</anchorfile>
      <anchor>adc35370b803782f896c844976a9ad6ce</anchor>
      <arglist>(float value, const char *unit, int precision)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>jxct_ui_system.cpp</name>
    <path>src/</path>
    <filename>jxct__ui__system_8cpp.html</filename>
    <includes id="jxct__ui__system_8h" name="jxct_ui_system.h" local="yes" import="no" module="no" objc="no">jxct_ui_system.h</includes>
    <member kind="function">
      <type>const char *</type>
      <name>getUnifiedCSS</name>
      <anchorfile>jxct__ui__system_8cpp.html</anchorfile>
      <anchor>a303ae6cee9693130ac22414e47140a1e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generateButton</name>
      <anchorfile>jxct__ui__system_8cpp.html</anchorfile>
      <anchor>ab544fbd8eec22a6da64c9c804981fa3e</anchor>
      <arglist>(ButtonType type, const ButtonConfig &amp;config)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>getToastHTML</name>
      <anchorfile>jxct__ui__system_8cpp.html</anchorfile>
      <anchor>afbda6d57dd082e7767c2c5e823951472</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>getLoaderHTML</name>
      <anchorfile>jxct__ui__system_8cpp.html</anchorfile>
      <anchor>a3a0ab8552fa88451ee4451e40d2058b3</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>logger.cpp</name>
    <path>src/</path>
    <filename>logger_8cpp.html</filename>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">../include/logger.h</includes>
    <member kind="function">
      <type>String</type>
      <name>formatLogMessage</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>a73d33bdd74467a1ac7814b1a783f7f28</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logError</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>a646cbac39f467aafa298c8dae1ff0091</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logWarn</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>a70f38e65c2a4620bbf763080674010af</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logInfo</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>a374e6f13f686b3de469c00c01d5fe1d3</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logDebug</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>aade0576f178f655d0f991ed324fd27ff</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logSuccess</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>a33d6cac9c78346491213dac197c0f9f3</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logSensor</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>ad52356734897533dd1ec07d78670a5bb</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logWiFi</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>abc98ed88066e0a808bb68d9a88ffa9c1</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logMQTT</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>ae3ae977a43dcf87df0dc3035a2983455</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logHTTP</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>a8d16d7846a1d3892a73f05af6286c36e</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logSystem</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>a9b60e3dbf53fc95c5a1ca77a6fec2643</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logData</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>a994321edf88e16ea088cdbaf0f8a8177</anchor>
      <arglist>(const String &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logSeparator</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>a33f45b00aaf104a528bb909059246d11</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logNewline</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>ac14de3d3a99ad0c7f8ff459619f12278</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logMemoryUsage</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>ab014807afa04f9783ed19c897cbdf54d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logSystemInfo</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>a36865f00b6f06ee84213f5a5a1c3c45d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logUptime</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>a07e106e3f19501a64fde4b0c6fa8d73a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logWiFiStatus</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>abf70e1b094519276f0363ac763f100ab</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setLogColor</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>abbb1feb235829ffa762eb8d2615b4864</anchor>
      <arglist>(LogColor color)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resetLogColor</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>abe2743f8bba1e6d306e147b47afb0943</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>getColorCode</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>ab94ddc842b6a690863609c28cfa93590</anchor>
      <arglist>(LogColor color)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printHeader</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>adc424edacf4b7c29b2a123afe3cbf575</anchor>
      <arglist>(const String &amp;title, LogColor color)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printSubHeader</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>acf4270ba81bcf41a83b9c6822df051b8</anchor>
      <arglist>(const String &amp;title, LogColor color)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printTimeStamp</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>afff4c4e20076552958d756dbcdcbe366</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>LogLevel</type>
      <name>currentLogLevel</name>
      <anchorfile>logger_8cpp.html</anchorfile>
      <anchor>a16cca5b581e7f50a9cc5e838510ae6f5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>main.cpp</name>
    <path>src/</path>
    <filename>main_8cpp.html</filename>
    <includes id="advanced__filters_8h" name="advanced_filters.h" local="yes" import="no" module="no" objc="no">advanced_filters.h</includes>
    <includes id="crop__recommendation__engine_8h" name="crop_recommendation_engine.h" local="yes" import="no" module="no" objc="no">business/crop_recommendation_engine.h</includes>
    <includes id="src_2business_2sensor__calibration__service_8h" name="sensor_calibration_service.h" local="yes" import="no" module="no" objc="no">business/sensor_calibration_service.h</includes>
    <includes id="sensor__compensation__service_8h" name="sensor_compensation_service.h" local="yes" import="no" module="no" objc="no">business/sensor_compensation_service.h</includes>
    <includes id="debug_8h" name="debug.h" local="yes" import="no" module="no" objc="no">debug.h</includes>
    <includes id="fake__sensor_8h" name="fake_sensor.h" local="yes" import="no" module="no" objc="no">fake_sensor.h</includes>
    <includes id="jxct__config__vars_8h" name="jxct_config_vars.h" local="yes" import="no" module="no" objc="no">jxct_config_vars.h</includes>
    <includes id="jxct__constants_8h" name="jxct_constants.h" local="yes" import="no" module="no" objc="no">jxct_constants.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">logger.h</includes>
    <includes id="modbus__sensor_8h" name="modbus_sensor.h" local="yes" import="no" module="no" objc="no">modbus_sensor.h</includes>
    <includes id="mqtt__client_8h" name="mqtt_client.h" local="yes" import="no" module="no" objc="no">mqtt_client.h</includes>
    <includes id="ota__manager_8h" name="ota_manager.h" local="yes" import="no" module="no" objc="no">ota_manager.h</includes>
    <includes id="sensor__factory_8h" name="sensor_factory.h" local="yes" import="no" module="no" objc="no">sensor_factory.h</includes>
    <includes id="thingspeak__client_8h" name="thingspeak_client.h" local="yes" import="no" module="no" objc="no">thingspeak_client.h</includes>
    <includes id="version_8h" name="version.h" local="yes" import="no" module="no" objc="no">version.h</includes>
    <includes id="web__routes_8h" name="web_routes.h" local="yes" import="no" module="no" objc="no">web_routes.h</includes>
    <includes id="wifi__manager_8h" name="wifi_manager.h" local="yes" import="no" module="no" objc="no">wifi_manager.h</includes>
    <member kind="function">
      <type>WebServer</type>
      <name>server</name>
      <anchorfile>main_8cpp.html</anchorfile>
      <anchor>a48f3f185be3ea2d153c347a157b38d42</anchor>
      <arglist>(80)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setup</name>
      <anchorfile>main_8cpp.html</anchorfile>
      <anchor>a4fc01d736fe50cf5b977f755b675f11d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>loop</name>
      <anchorfile>main_8cpp.html</anchorfile>
      <anchor>afe461d27b9c48d5921c00d521181f12f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>CropRecommendationEngine</type>
      <name>gCropEngine</name>
      <anchorfile>main_8cpp.html</anchorfile>
      <anchor>a846779af5e07f808b2cee49916205a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SensorCalibrationService</type>
      <name>gCalibrationService</name>
      <anchorfile>main_8cpp.html</anchorfile>
      <anchor>a2048f65bc87ffffe1cdadbacc0e95a58</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SensorCompensationService</type>
      <name>gCompensationService</name>
      <anchorfile>main_8cpp.html</anchorfile>
      <anchor>a18619e11a2ee0b61db56d4fd39fbc6c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>WiFiUDP</type>
      <name>ntpUDP</name>
      <anchorfile>main_8cpp.html</anchorfile>
      <anchor>a22f33e7e05df58bb6145bb6e543e232a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>NTPClient *</type>
      <name>timeClient</name>
      <anchorfile>main_8cpp.html</anchorfile>
      <anchor>ab4cd0eb2684ad54d1d0782f4ff75e07f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>modbus_sensor.cpp</name>
    <path>src/</path>
    <filename>modbus__sensor_8cpp.html</filename>
    <includes id="modbus__sensor_8h" name="modbus_sensor.h" local="yes" import="no" module="no" objc="no">modbus_sensor.h</includes>
    <includes id="advanced__filters_8h" name="advanced_filters.h" local="yes" import="no" module="no" objc="no">advanced_filters.h</includes>
    <includes id="business__services_8h" name="business_services.h" local="yes" import="no" module="no" objc="no">business_services.h</includes>
    <includes id="src_2calibration__manager_8h" name="calibration_manager.h" local="yes" import="no" module="no" objc="no">calibration_manager.h</includes>
    <includes id="debug_8h" name="debug.h" local="yes" import="no" module="no" objc="no">debug.h</includes>
    <includes id="jxct__config__vars_8h" name="jxct_config_vars.h" local="yes" import="no" module="no" objc="no">jxct_config_vars.h</includes>
    <includes id="jxct__constants_8h" name="jxct_constants.h" local="yes" import="no" module="no" objc="no">jxct_constants.h</includes>
    <includes id="jxct__device__info_8h" name="jxct_device_info.h" local="yes" import="no" module="no" objc="no">jxct_device_info.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">logger.h</includes>
    <includes id="sensor__compensation_8h" name="sensor_compensation.h" local="yes" import="no" module="no" objc="no">sensor_compensation.h</includes>
    <includes id="validation__utils_8h" name="validation_utils.h" local="yes" import="no" module="no" objc="no">validation_utils.h</includes>
    <member kind="function">
      <type>void</type>
      <name>testSP3485E</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>ab8a1f5220dc77f33a89d14fe4d82442f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupModbus</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>a0c4410314e55fce67a3c727cb1bf9520</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>validateSensorData</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>aa8a3b28ec8c21261d4e467da91a17a67</anchor>
      <arglist>(SensorData &amp;data)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>getCachedData</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>a8eb8daa78bacfc8a930012162882aa9c</anchor>
      <arglist>(SensorData &amp;data)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readFirmwareVersion</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>a6444c2d436a57168cc75d4ef8529a6c8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readErrorStatus</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>a8b6b2ad640c9580fa45d8cbad28adf53</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>testModbusConnection</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>aedb09484ba8020afc57e9e9e26012bf1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>readSensorData</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>ac3a37446a712d9ae94e76304361d35d2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>preTransmission</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>a1c43d8a4afead0d0df29635242290377</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>postTransmission</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>a33b535e84f654e423bb627b25b7b3759</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>realSensorTask</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>ae64cda2b754416ebb3bcb79f98176319</anchor>
      <arglist>(void *)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>startRealSensorTask</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>a4f1a431da8f1feb8a64e2c5f887056df</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printModbusError</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>adbf50656913748323cd03274af03c116</anchor>
      <arglist>(uint8_t errNum)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initMovingAverageBuffers</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>ae316f6943acb0b355ff8b0f878b08d04</anchor>
      <arglist>(SensorData &amp;data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addToMovingAverage</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>aa3a75ec1c2e64f7a20f651c168e3f82b</anchor>
      <arglist>(SensorData &amp;data, const SensorData &amp;newReading)</arglist>
    </member>
    <member kind="function">
      <type>SensorData</type>
      <name>getSensorData</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>a3fac607750afe2d74837248aeec2cfe0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>ModbusMaster &amp;</type>
      <name>getModbus</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>a3008e41a02a9857f034bc685d5aadbc0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>getSensorLastError</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>a3ff54b4bc25eb08e654e0ddbc9d998d0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>SensorData &amp;</type>
      <name>getSensorDataRef</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>a353fd147ac312a9252179d62f52eb595</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>SensorCache &amp;</type>
      <name>getSensorCache</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>a0174826352a790616d0ad5c414972e68</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>SensorData</type>
      <name>sensorData</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>ae6ed671f3036da58a93991fca2a2329c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SensorCache</type>
      <name>sensorCache</name>
      <anchorfile>modbus__sensor_8cpp.html</anchorfile>
      <anchor>a5a90cc33c11a27aac5f56dcaac387c0d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>modbus_sensor.h</name>
    <path>src/</path>
    <filename>modbus__sensor_8h.html</filename>
    <includes id="jxct__constants_8h" name="jxct_constants.h" local="yes" import="no" module="no" objc="no">jxct_constants.h</includes>
    <class kind="struct">SensorData</class>
    <class kind="struct">SensorCache</class>
    <member kind="define">
      <type>#define</type>
      <name>REG_PH</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a1f0d81a7889405dddfd0740779103c1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>REG_SOIL_MOISTURE</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>ae81f2380d8a1a56150fef81dfff25cfc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>REG_SOIL_TEMP</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a183e5bdeda520ac205c6fa93a52e1ef4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>REG_CONDUCTIVITY</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>ae4a11435c54c0c61a370f01ad74783dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>REG_NITROGEN</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a9f80ba4d6b98c8b586f32a67189637c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>REG_PHOSPHORUS</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a6aa33492b3c99407fab5717bf51f3c58</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>REG_POTASSIUM</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a2f35ba14a2dedd4527dadfe5df270ecc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>REG_FIRMWARE_VERSION</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a149baf37365c4dd67e8c37e211fe5afc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>REG_CALIBRATION</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a9b707a93013b08d191303ed584a140d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>REG_ERROR_STATUS</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>ac264831b378bd2513785f56a63a2cf9a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>REG_DEVICE_ADDRESS</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a9261d699774c32e6612b86d5ff383120</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MIN_TEMPERATURE</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>ac5080f3f6bf208edce5de49e5117a5f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_TEMPERATURE</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>af900a8b6b1e1763dd0e90e48fc2fb317</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MIN_HUMIDITY</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>abc7849e08d6e99a7e151cb4366ebc96d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_HUMIDITY</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a4ae8ae01ff5a5ce2e90eca5fbc669d81</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MIN_EC</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>ac6595c2759ec7982567a7648cc513465</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_EC</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a8060470295f959c8f0f60905d04691c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MIN_PH</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>aeb31a7b73c962d56d9fca3209a95723e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_PH</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a6035bff843569e704debcabd15c481d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MIN_NPK</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>ad4ce924e7edbb024c3f5f25c42d64eca</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_NPK</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a0db0c07c73c5ce97b292e0ff8414da3d</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>String &amp;</type>
      <name>getSensorLastError</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a3ff54b4bc25eb08e654e0ddbc9d998d0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>SensorData</type>
      <name>getSensorData</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a3fac607750afe2d74837248aeec2cfe0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupModbus</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a0c4410314e55fce67a3c727cb1bf9520</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>readSensorData</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>ac3a37446a712d9ae94e76304361d35d2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readFirmwareVersion</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a6444c2d436a57168cc75d4ef8529a6c8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>readErrorStatus</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a8b6b2ad640c9580fa45d8cbad28adf53</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>changeDeviceAddress</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>aac87bc87eaf0bc01ebda5fe1bb44c88b</anchor>
      <arglist>(uint8_t new_address)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>validateSensorData</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>aa8a3b28ec8c21261d4e467da91a17a67</anchor>
      <arglist>(SensorData &amp;data)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>getCachedData</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a8eb8daa78bacfc8a930012162882aa9c</anchor>
      <arglist>(SensorData &amp;data)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>convertRegisterToFloat</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>aca7d239567ef1281e0fd378082fd48ac</anchor>
      <arglist>(uint16_t value, float multiplier)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>preTransmission</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a1c43d8a4afead0d0df29635242290377</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>postTransmission</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a33b535e84f654e423bb627b25b7b3759</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>printModbusError</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>adbf50656913748323cd03274af03c116</anchor>
      <arglist>(uint8_t errNum)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>startRealSensorTask</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a4f1a431da8f1feb8a64e2c5f887056df</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addToMovingAverage</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>aa3a75ec1c2e64f7a20f651c168e3f82b</anchor>
      <arglist>(SensorData &amp;data, const SensorData &amp;newReading)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>calculateMovingAverage</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>adacc51c210f3e18552641449ebdcc4a4</anchor>
      <arglist>(const float *buffer, uint8_t window_size, uint8_t filled)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initMovingAverageBuffers</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>ae316f6943acb0b355ff8b0f878b08d04</anchor>
      <arglist>(SensorData &amp;data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testSP3485E</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>ab8a1f5220dc77f33a89d14fe4d82442f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>testModbusConnection</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>aedb09484ba8020afc57e9e9e26012bf1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>testSerialConfigurations</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a372e89ffe0abcaf103578a478d3b4b12</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>SensorData</type>
      <name>sensorData</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>ae6ed671f3036da58a93991fca2a2329c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>SensorCache</type>
      <name>sensorCache</name>
      <anchorfile>modbus__sensor_8h.html</anchorfile>
      <anchor>a5a90cc33c11a27aac5f56dcaac387c0d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>modbus_sensor_adapter.h</name>
    <path>src/</path>
    <filename>modbus__sensor__adapter_8h.html</filename>
    <includes id="basic__sensor__adapter_8h" name="basic_sensor_adapter.h" local="yes" import="no" module="no" objc="no">basic_sensor_adapter.h</includes>
    <class kind="class">ModbusSensorAdapter</class>
  </compound>
  <compound kind="file">
    <name>mqtt_client.cpp</name>
    <path>src/</path>
    <filename>mqtt__client_8cpp.html</filename>
    <includes id="mqtt__client_8h" name="mqtt_client.h" local="yes" import="no" module="no" objc="no">mqtt_client.h</includes>
    <includes id="debug_8h" name="debug.h" local="yes" import="no" module="no" objc="no">debug.h</includes>
    <includes id="jxct__config__vars_8h" name="jxct_config_vars.h" local="yes" import="no" module="no" objc="no">jxct_config_vars.h</includes>
    <includes id="jxct__constants_8h" name="jxct_constants.h" local="yes" import="no" module="no" objc="no">jxct_constants.h</includes>
    <includes id="jxct__device__info_8h" name="jxct_device_info.h" local="yes" import="no" module="no" objc="no">jxct_device_info.h</includes>
    <includes id="jxct__format__utils_8h" name="jxct_format_utils.h" local="yes" import="no" module="no" objc="no">jxct_format_utils.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">logger.h</includes>
    <includes id="modbus__sensor_8h" name="modbus_sensor.h" local="yes" import="no" module="no" objc="no">modbus_sensor.h</includes>
    <includes id="ota__manager_8h" name="ota_manager.h" local="yes" import="no" module="no" objc="no">ota_manager.h</includes>
    <includes id="wifi__manager_8h" name="wifi_manager.h" local="yes" import="no" module="no" objc="no">wifi_manager.h</includes>
    <member kind="function">
      <type>PubSubClient</type>
      <name>mqttClient</name>
      <anchorfile>mqtt__client_8cpp.html</anchorfile>
      <anchor>a717105311582e81f1c9c6bb2238a00d2</anchor>
      <arglist>(espClient)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>publishAvailability</name>
      <anchorfile>mqtt__client_8cpp.html</anchorfile>
      <anchor>aaa9510911ee8e47743efa916df00c13b</anchor>
      <arglist>(bool online)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupMQTT</name>
      <anchorfile>mqtt__client_8cpp.html</anchorfile>
      <anchor>a3cff6686f09444558b7c9436e082e73c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>connectMQTT</name>
      <anchorfile>mqtt__client_8cpp.html</anchorfile>
      <anchor>ab5a788a03971cae1e5e641ae9b1d7fe0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleMQTT</name>
      <anchorfile>mqtt__client_8cpp.html</anchorfile>
      <anchor>a926aeaa7d52ebd60e289af8d6ef98600</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>publishSensorData</name>
      <anchorfile>mqtt__client_8cpp.html</anchorfile>
      <anchor>a9d23c1c2b3b5f9613a2215945494badb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>publishHomeAssistantConfig</name>
      <anchorfile>mqtt__client_8cpp.html</anchorfile>
      <anchor>ac49364afe5be3cf4e718ab09d67017da</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>removeHomeAssistantConfig</name>
      <anchorfile>mqtt__client_8cpp.html</anchorfile>
      <anchor>a7a9b6d18074276b45340af02c443924a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleMqttCommand</name>
      <anchorfile>mqtt__client_8cpp.html</anchorfile>
      <anchor>a58d6760f5e7f2aaf28fbb2155942d579</anchor>
      <arglist>(const String &amp;cmd)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>mqttCallback</name>
      <anchorfile>mqtt__client_8cpp.html</anchorfile>
      <anchor>aee48b9097aaffe8238bbeae09e8fd944</anchor>
      <arglist>(const char *topic, const byte *payload, unsigned int length)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>invalidateHAConfigCache</name>
      <anchorfile>mqtt__client_8cpp.html</anchorfile>
      <anchor>a78b590745e792368420169e642494895</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>getMqttLastError</name>
      <anchorfile>mqtt__client_8cpp.html</anchorfile>
      <anchor>ae7138b08148b127ef0202b5cc7ad389e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>NTPClient *</type>
      <name>timeClient</name>
      <anchorfile>mqtt__client_8cpp.html</anchorfile>
      <anchor>ab4cd0eb2684ad54d1d0782f4ff75e07f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>WiFiClient</type>
      <name>espClient</name>
      <anchorfile>mqtt__client_8cpp.html</anchorfile>
      <anchor>abd77e757e4b3bb6f1e4b42b21ea9e040</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>mqtt_client.h</name>
    <path>src/</path>
    <filename>mqtt__client_8h.html</filename>
    <member kind="function">
      <type>const char *</type>
      <name>getMqttLastError</name>
      <anchorfile>mqtt__client_8h.html</anchorfile>
      <anchor>ae7138b08148b127ef0202b5cc7ad389e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupMQTT</name>
      <anchorfile>mqtt__client_8h.html</anchorfile>
      <anchor>a3cff6686f09444558b7c9436e082e73c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>connectMQTT</name>
      <anchorfile>mqtt__client_8h.html</anchorfile>
      <anchor>ab5a788a03971cae1e5e641ae9b1d7fe0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleMQTT</name>
      <anchorfile>mqtt__client_8h.html</anchorfile>
      <anchor>a926aeaa7d52ebd60e289af8d6ef98600</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>publishSensorData</name>
      <anchorfile>mqtt__client_8h.html</anchorfile>
      <anchor>a9d23c1c2b3b5f9613a2215945494badb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>publishHomeAssistantConfig</name>
      <anchorfile>mqtt__client_8h.html</anchorfile>
      <anchor>ac49364afe5be3cf4e718ab09d67017da</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>publishAvailability</name>
      <anchorfile>mqtt__client_8h.html</anchorfile>
      <anchor>aaa9510911ee8e47743efa916df00c13b</anchor>
      <arglist>(bool online)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>removeHomeAssistantConfig</name>
      <anchorfile>mqtt__client_8h.html</anchorfile>
      <anchor>a7a9b6d18074276b45340af02c443924a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>invalidateHAConfigCache</name>
      <anchorfile>mqtt__client_8h.html</anchorfile>
      <anchor>a78b590745e792368420169e642494895</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleMqttCommand</name>
      <anchorfile>mqtt__client_8h.html</anchorfile>
      <anchor>a58d6760f5e7f2aaf28fbb2155942d579</anchor>
      <arglist>(const String &amp;cmd)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>mqttCallback</name>
      <anchorfile>mqtt__client_8h.html</anchorfile>
      <anchor>aee48b9097aaffe8238bbeae09e8fd944</anchor>
      <arglist>(const char *topic, const byte *payload, unsigned int length)</arglist>
    </member>
    <member kind="variable">
      <type>WiFiClient</type>
      <name>espClient</name>
      <anchorfile>mqtt__client_8h.html</anchorfile>
      <anchor>abd77e757e4b3bb6f1e4b42b21ea9e040</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>PubSubClient</type>
      <name>mqttClient</name>
      <anchorfile>mqtt__client_8h.html</anchorfile>
      <anchor>a86d63f481644da70f063c2593fcafe19</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>mqttConnected</name>
      <anchorfile>mqtt__client_8h.html</anchorfile>
      <anchor>a1da8e8d4c027f4c08db8b493b99e12da</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ota_manager.cpp</name>
    <path>src/</path>
    <filename>ota__manager_8cpp.html</filename>
    <includes id="ota__manager_8h" name="ota_manager.h" local="yes" import="no" module="no" objc="no">ota_manager.h</includes>
    <includes id="jxct__config__vars_8h" name="jxct_config_vars.h" local="yes" import="no" module="no" objc="no">jxct_config_vars.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">logger.h</includes>
    <includes id="version_8h" name="version.h" local="yes" import="no" module="no" objc="no">version.h</includes>
    <member kind="function">
      <type>void</type>
      <name>checkGuard</name>
      <anchorfile>ota__manager_8cpp.html</anchorfile>
      <anchor>ace424db218f3dfab484ebd7f61d44fcf</anchor>
      <arglist>(const char *tag)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>getOtaStatus</name>
      <anchorfile>ota__manager_8cpp.html</anchorfile>
      <anchor>a51f9f9bafeb2f56a3456ebf48a76c7b9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupOTA</name>
      <anchorfile>ota__manager_8cpp.html</anchorfile>
      <anchor>a8061d13b589bae87956ec09e4ea7fef5</anchor>
      <arglist>(const char *manifestUrl, WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>verifySha256</name>
      <anchorfile>ota__manager_8cpp.html</anchorfile>
      <anchor>a2b7fa9588e9d775a9058925bb170f465</anchor>
      <arglist>(const uint8_t *calcDigest, const char *expectedHex)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>downloadAndUpdate</name>
      <anchorfile>ota__manager_8cpp.html</anchorfile>
      <anchor>a79a35cc6faa209f36bab011a327fb463</anchor>
      <arglist>(const String &amp;binUrl, const char *expectedSha256)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>triggerOtaCheck</name>
      <anchorfile>ota__manager_8cpp.html</anchorfile>
      <anchor>aab62f74691b98553d395b694e0807ae4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>triggerOtaInstall</name>
      <anchorfile>ota__manager_8cpp.html</anchorfile>
      <anchor>a2463eda06898e53e0ca3b3a819c995ae</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleOTA</name>
      <anchorfile>ota__manager_8cpp.html</anchorfile>
      <anchor>a3caeaca3fcbd288827f42df9af8b7f08</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sensor_compensation.cpp</name>
    <path>src/</path>
    <filename>sensor__compensation_8cpp.html</filename>
    <includes id="sensor__compensation_8h" name="sensor_compensation.h" local="yes" import="no" module="no" objc="no">sensor_compensation.h</includes>
    <includes id="jxct__config__vars_8h" name="jxct_config_vars.h" local="yes" import="no" module="no" objc="no">jxct_config_vars.h</includes>
    <member kind="function">
      <type>float</type>
      <name>correctEC</name>
      <anchorfile>sensor__compensation_8cpp.html</anchorfile>
      <anchor>a227ff4e52c125fb1ef8dfcffce04c9b6</anchor>
      <arglist>(float ecRaw, float temperature, float compensationFactor)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>correctPH</name>
      <anchorfile>sensor__compensation_8cpp.html</anchorfile>
      <anchor>a941bde06e253ad4a143a22f9f6d9b8df</anchor>
      <arglist>(float rawValue, float temperature, float compensationFactor)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>correctPH</name>
      <anchorfile>sensor__compensation_8cpp.html</anchorfile>
      <anchor>ab7723d1457f8d7b7b91a31c58ba1c328</anchor>
      <arglist>(float rawValue, float temperature)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>correctNPK</name>
      <anchorfile>sensor__compensation_8cpp.html</anchorfile>
      <anchor>ae358d68509b6fe2b2dee25422d8fc97a</anchor>
      <arglist>(float rawValue, float temperature, float humidity, float compensationFactor)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>correctEC</name>
      <anchorfile>sensor__compensation_8cpp.html</anchorfile>
      <anchor>afafc0921b1ae4ac19063de1828f7ac4b</anchor>
      <arglist>(float ecRaw, const EnvironmentalConditions &amp;env, SoilType soil)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>correctNPK</name>
      <anchorfile>sensor__compensation_8cpp.html</anchorfile>
      <anchor>a9406fc6bd5dad362d51fcf4547cd279a</anchor>
      <arglist>(const EnvironmentalConditions &amp;env, NPKReferences &amp;npk, SoilType soil)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>correctEC</name>
      <anchorfile>sensor__compensation_8cpp.html</anchorfile>
      <anchor>a1e1da2e71131eec41e98642456f51974</anchor>
      <arglist>(float ecRaw, float temperature, float theta, SoilType soil)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>correctNPK</name>
      <anchorfile>sensor__compensation_8cpp.html</anchorfile>
      <anchor>a343ee96fb5eebef782985e8ff97d3b9c</anchor>
      <arglist>(float temperature, float theta, SoilType soil, NPKReferences &amp;npk)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thingspeak_client.cpp</name>
    <path>src/</path>
    <filename>thingspeak__client_8cpp.html</filename>
    <includes id="thingspeak__client_8h" name="thingspeak_client.h" local="yes" import="no" module="no" objc="no">thingspeak_client.h</includes>
    <includes id="jxct__config__vars_8h" name="jxct_config_vars.h" local="yes" import="no" module="no" objc="no">jxct_config_vars.h</includes>
    <includes id="jxct__device__info_8h" name="jxct_device_info.h" local="yes" import="no" module="no" objc="no">jxct_device_info.h</includes>
    <includes id="jxct__format__utils_8h" name="jxct_format_utils.h" local="yes" import="no" module="no" objc="no">jxct_format_utils.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">logger.h</includes>
    <includes id="modbus__sensor_8h" name="modbus_sensor.h" local="yes" import="no" module="no" objc="no">modbus_sensor.h</includes>
    <includes id="wifi__manager_8h" name="wifi_manager.h" local="yes" import="no" module="no" objc="no">wifi_manager.h</includes>
    <member kind="function">
      <type>const char *</type>
      <name>getThingSpeakLastPublish</name>
      <anchorfile>thingspeak__client_8cpp.html</anchorfile>
      <anchor>add7f50ba743d6501c1ad5e2dd4985e53</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>getThingSpeakLastError</name>
      <anchorfile>thingspeak__client_8cpp.html</anchorfile>
      <anchor>ae3f256ffaf18113d28229c8ae013b1f5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupThingSpeak</name>
      <anchorfile>thingspeak__client_8cpp.html</anchorfile>
      <anchor>af8e20ef000ee56a6aedcc07d88f38f90</anchor>
      <arglist>(WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>sendDataToThingSpeak</name>
      <anchorfile>thingspeak__client_8cpp.html</anchorfile>
      <anchor>aaef2ba5628e59244986aa0c0f40b58cf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>NTPClient *</type>
      <name>timeClient</name>
      <anchorfile>thingspeak__client_8cpp.html</anchorfile>
      <anchor>ab4cd0eb2684ad54d1d0782f4ff75e07f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>thingspeak_client.h</name>
    <path>src/</path>
    <filename>thingspeak__client_8h.html</filename>
    <member kind="function">
      <type>const char *</type>
      <name>getThingSpeakLastPublish</name>
      <anchorfile>thingspeak__client_8h.html</anchorfile>
      <anchor>add7f50ba743d6501c1ad5e2dd4985e53</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>getThingSpeakLastError</name>
      <anchorfile>thingspeak__client_8h.html</anchorfile>
      <anchor>ae3f256ffaf18113d28229c8ae013b1f5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupThingSpeak</name>
      <anchorfile>thingspeak__client_8h.html</anchorfile>
      <anchor>af8e20ef000ee56a6aedcc07d88f38f90</anchor>
      <arglist>(WiFiClient &amp;client)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>sendDataToThingSpeak</name>
      <anchorfile>thingspeak__client_8h.html</anchorfile>
      <anchor>aaef2ba5628e59244986aa0c0f40b58cf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>WiFiClient</type>
      <name>espClient</name>
      <anchorfile>thingspeak__client_8h.html</anchorfile>
      <anchor>abd77e757e4b3bb6f1e4b42b21ea9e040</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>validation_utils.cpp</name>
    <path>src/</path>
    <filename>validation__utils_8cpp.html</filename>
    <includes id="validation__utils_8h" name="validation_utils.h" local="yes" import="no" module="no" objc="no">validation_utils.h</includes>
    <includes id="jxct__constants_8h" name="jxct_constants.h" local="yes" import="no" module="no" objc="no">jxct_constants.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">logger.h</includes>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateInterval</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>a1a1e770ad345dd09684dbc5cf9fea341</anchor>
      <arglist>(unsigned long interval, unsigned long min_val, unsigned long max_val, const char *field_name)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateSensorReadInterval</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>a7f07d4d6084e7aa1d2de3a8b5661d9e3</anchor>
      <arglist>(unsigned long interval)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateMQTTPublishInterval</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>a15f556efd2c016821186e4604e1d94a5</anchor>
      <arglist>(unsigned long interval)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateThingSpeakInterval</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>a5b75ecc2060e8847777ac37a7b17baf2</anchor>
      <arglist>(unsigned long interval)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateNTPInterval</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>affc3bd3259b06ba5eaaec72c6196886d</anchor>
      <arglist>(unsigned long interval)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateRange</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>aee1ec6b01bc6185c471610c55ff89fb2</anchor>
      <arglist>(float value, float min_val, float max_val, const char *field_name)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateTemperature</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>afe3a5befa4789b9ca6c7655d6281f888</anchor>
      <arglist>(float temperature)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateHumidity</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>a704129fdab28312c377892a04c574a4b</anchor>
      <arglist>(float humidity)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validatePH</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>ace3036c7f158287e76837d83a09fac8a</anchor>
      <arglist>(float phValue)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateEC</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>a49ce4c78f239a0f993d8fd96dcb4e3e8</anchor>
      <arglist>(float ecValue)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateNPK</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>a60ce7cf3c6ae19175789b058eaaede80</anchor>
      <arglist>(float value, const char *nutrient)</arglist>
    </member>
    <member kind="function">
      <type>ConfigValidationResult</type>
      <name>validateFullConfig</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>aee4e9769246c3d2198f6aaa193cce9e1</anchor>
      <arglist>(const ConfigData &amp;config, bool checkRequired)</arglist>
    </member>
    <member kind="function">
      <type>SensorValidationResult</type>
      <name>validateFullSensorData</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>a1b8529ff5465c73b043d27ad7d2e86f3</anchor>
      <arglist>(const SensorData &amp;data)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isValidIPAddress</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>a656f25f6039ad27197946f611133158a</anchor>
      <arglist>(const String &amp;ipAddress)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isValidHostname</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>abf0e43fe7c5fd9b3439d01e4a0557259</anchor>
      <arglist>(const String &amp;hostname)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>formatValidationErrors</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>af447043f0553ccee92c72fb59009b1ec</anchor>
      <arglist>(const ConfigValidationResult &amp;result)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>formatSensorValidationErrors</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>aecb050d7cbd49c3a3cb0dadcb72360c3</anchor>
      <arglist>(const SensorValidationResult &amp;result)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logValidationResult</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>a5b0b5f3adf97cc7910641d046b3e292c</anchor>
      <arglist>(const ConfigValidationResult &amp;result, const char *context)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logSensorValidationResult</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>a90392289dbf6077016cdb0ce0cdc34f8</anchor>
      <arglist>(const SensorValidationResult &amp;result, const char *context)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateSSID</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>af988ebb1f8e6e907df08f24a81d34c6b</anchor>
      <arglist>(const String &amp;ssid)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validatePassword</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>a9e87dd09b18043e14aeaf09bb1ed4577</anchor>
      <arglist>(const String &amp;password)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateMQTTServer</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>aa496a6a13d80b8662e7dfea4fd9df479</anchor>
      <arglist>(const String &amp;server)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateMQTTPort</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>a47bd7b80bf2f4f9b117609e7e1cc507c</anchor>
      <arglist>(int port)</arglist>
    </member>
    <member kind="function">
      <type>ValidationResult</type>
      <name>validateThingSpeakAPIKey</name>
      <anchorfile>validation__utils_8cpp.html</anchorfile>
      <anchor>aed42aace8ccac73c73eb1440f8cd2afc</anchor>
      <arglist>(const String &amp;apiKey)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>csrf_protection.cpp</name>
    <path>src/web/</path>
    <filename>csrf__protection_8cpp.html</filename>
    <includes id="csrf__protection_8h" name="csrf_protection.h" local="yes" import="no" module="no" objc="no">../../include/web/csrf_protection.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">../../include/logger.h</includes>
    <member kind="function">
      <type>String</type>
      <name>generateCSRFToken</name>
      <anchorfile>csrf__protection_8cpp.html</anchorfile>
      <anchor>ab27a975e89338b0b23a40e028b1ae9b2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>validateCSRFToken</name>
      <anchorfile>csrf__protection_8cpp.html</anchorfile>
      <anchor>a936ecd18be81531b306c2c4705f7e860</anchor>
      <arglist>(const String &amp;token)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>getCSRFHiddenField</name>
      <anchorfile>csrf__protection_8cpp.html</anchorfile>
      <anchor>aab8be89322026594573b8f3c8064bbbc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>checkCSRFSafety</name>
      <anchorfile>csrf__protection_8cpp.html</anchorfile>
      <anchor>a0320c55f4de318df92de8a73e8e7d828</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>methodToString</name>
      <anchorfile>csrf__protection_8cpp.html</anchorfile>
      <anchor>ad9d33eff9bf83f09111cc522049930bc</anchor>
      <arglist>(HTTPMethod method)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initCSRFProtection</name>
      <anchorfile>csrf__protection_8cpp.html</anchorfile>
      <anchor>ac922ac6444b8b990e13e3746ca931065</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>const unsigned long</type>
      <name>CSRF_TOKEN_LIFETIME</name>
      <anchorfile>csrf__protection_8cpp.html</anchorfile>
      <anchor>a0857d7f29ac875e2258e9bd3487e6ce0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>error_handlers.cpp</name>
    <path>src/web/</path>
    <filename>error__handlers_8cpp.html</filename>
    <includes id="jxct__config__vars_8h" name="jxct_config_vars.h" local="yes" import="no" module="no" objc="no">../../include/jxct_config_vars.h</includes>
    <includes id="jxct__strings_8h" name="jxct_strings.h" local="yes" import="no" module="no" objc="no">../../include/jxct_strings.h</includes>
    <includes id="jxct__ui__system_8h" name="jxct_ui_system.h" local="yes" import="no" module="no" objc="no">../../include/jxct_ui_system.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">../../include/logger.h</includes>
    <includes id="web__routes_8h" name="web_routes.h" local="yes" import="no" module="no" objc="no">../../include/web_routes.h</includes>
    <includes id="wifi__manager_8h" name="wifi_manager.h" local="yes" import="no" module="no" objc="no">../wifi_manager.h</includes>
    <class kind="struct">ValidationRange</class>
    <class kind="struct">ValidationRange::Builder</class>
    <class kind="struct">HttpRequest</class>
    <class kind="struct">HttpRequest::Builder</class>
    <member kind="function">
      <type>void</type>
      <name>setupErrorHandlers</name>
      <anchorfile>error__handlers_8cpp.html</anchorfile>
      <anchor>a833be2e68cb85aa95168dae9c9b128ef</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>validateConfigInput</name>
      <anchorfile>error__handlers_8cpp.html</anchorfile>
      <anchor>a15eab9f9a3dc97d41a860ab966800fa6</anchor>
      <arglist>(bool checkRequired)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isFeatureAvailable</name>
      <anchorfile>error__handlers_8cpp.html</anchorfile>
      <anchor>aef0848857422724f7d825dd8a33421b9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generateValidationErrorResponse</name>
      <anchorfile>error__handlers_8cpp.html</anchorfile>
      <anchor>a6fe24c5dc21b012791aecd16ab4dad96</anchor>
      <arglist>(const String &amp;errorMsg)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleCriticalError</name>
      <anchorfile>error__handlers_8cpp.html</anchorfile>
      <anchor>a200ff078a206ee5a0b44bc0c5411f3f5</anchor>
      <arglist>(const String &amp;error)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isRouteAvailable</name>
      <anchorfile>error__handlers_8cpp.html</anchorfile>
      <anchor>a195f4874889e332e6f540a518d6c51fe</anchor>
      <arglist>(const String &amp;uri)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>routes_config.cpp</name>
    <path>src/web/</path>
    <filename>routes__config_8cpp.html</filename>
    <includes id="jxct__config__vars_8h" name="jxct_config_vars.h" local="yes" import="no" module="no" objc="no">../../include/jxct_config_vars.h</includes>
    <includes id="jxct__constants_8h" name="jxct_constants.h" local="yes" import="no" module="no" objc="no">../../include/jxct_constants.h</includes>
    <includes id="jxct__device__info_8h" name="jxct_device_info.h" local="yes" import="no" module="no" objc="no">../../include/jxct_device_info.h</includes>
    <includes id="jxct__strings_8h" name="jxct_strings.h" local="yes" import="no" module="no" objc="no">../../include/jxct_strings.h</includes>
    <includes id="jxct__ui__system_8h" name="jxct_ui_system.h" local="yes" import="no" module="no" objc="no">../../include/jxct_ui_system.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">../../include/logger.h</includes>
    <includes id="validation__utils_8h" name="validation_utils.h" local="yes" import="no" module="no" objc="no">../../include/validation_utils.h</includes>
    <includes id="csrf__protection_8h" name="csrf_protection.h" local="yes" import="no" module="no" objc="no">../../include/web/csrf_protection.h</includes>
    <includes id="web__routes_8h" name="web_routes.h" local="yes" import="no" module="no" objc="no">../../include/web_routes.h</includes>
    <includes id="wifi__manager_8h" name="wifi_manager.h" local="yes" import="no" module="no" objc="no">../wifi_manager.h</includes>
    <member kind="function">
      <type>void</type>
      <name>setupConfigRoutes</name>
      <anchorfile>routes__config_8cpp.html</anchorfile>
      <anchor>a0e7c19a3399afd6ceae18c4512cca24c</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>routes_data.cpp</name>
    <path>src/web/</path>
    <filename>routes__data_8cpp.html</filename>
    <includes id="jxct__config__vars_8h" name="jxct_config_vars.h" local="yes" import="no" module="no" objc="no">../../include/jxct_config_vars.h</includes>
    <includes id="jxct__constants_8h" name="jxct_constants.h" local="yes" import="no" module="no" objc="no">../../include/jxct_constants.h</includes>
    <includes id="jxct__format__utils_8h" name="jxct_format_utils.h" local="yes" import="no" module="no" objc="no">../../include/jxct_format_utils.h</includes>
    <includes id="jxct__strings_8h" name="jxct_strings.h" local="yes" import="no" module="no" objc="no">../../include/jxct_strings.h</includes>
    <includes id="jxct__ui__system_8h" name="jxct_ui_system.h" local="yes" import="no" module="no" objc="no">../../include/jxct_ui_system.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">../../include/logger.h</includes>
    <includes id="csrf__protection_8h" name="csrf_protection.h" local="yes" import="no" module="no" objc="no">../../include/web/csrf_protection.h</includes>
    <includes id="web__routes_8h" name="web_routes.h" local="yes" import="no" module="no" objc="no">../../include/web_routes.h</includes>
    <includes id="modbus__sensor_8h" name="modbus_sensor.h" local="yes" import="no" module="no" objc="no">../modbus_sensor.h</includes>
    <includes id="wifi__manager_8h" name="wifi_manager.h" local="yes" import="no" module="no" objc="no">../wifi_manager.h</includes>
    <includes id="business__services_8h" name="business_services.h" local="yes" import="no" module="no" objc="no">business_services.h</includes>
    <member kind="function">
      <type>void</type>
      <name>handleReadingsUpload</name>
      <anchorfile>routes__data_8cpp.html</anchorfile>
      <anchor>a43999630c470d7068023c4f423c0722c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sendSensorJson</name>
      <anchorfile>routes__data_8cpp.html</anchorfile>
      <anchor>a636822ce9463007c8c3e5b10d00b26e1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupDataRoutes</name>
      <anchorfile>routes__data_8cpp.html</anchorfile>
      <anchor>aed1136f683a386e56678e05fc3747972</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>NTPClient *</type>
      <name>timeClient</name>
      <anchorfile>routes__data_8cpp.html</anchorfile>
      <anchor>ab4cd0eb2684ad54d1d0782f4ff75e07f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>routes_main.cpp</name>
    <path>src/web/</path>
    <filename>routes__main_8cpp.html</filename>
    <includes id="jxct__config__vars_8h" name="jxct_config_vars.h" local="yes" import="no" module="no" objc="no">../../include/jxct_config_vars.h</includes>
    <includes id="jxct__constants_8h" name="jxct_constants.h" local="yes" import="no" module="no" objc="no">../../include/jxct_constants.h</includes>
    <includes id="jxct__ui__system_8h" name="jxct_ui_system.h" local="yes" import="no" module="no" objc="no">../../include/jxct_ui_system.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">../../include/logger.h</includes>
    <includes id="validation__utils_8h" name="validation_utils.h" local="yes" import="no" module="no" objc="no">../../include/validation_utils.h</includes>
    <includes id="web__routes_8h" name="web_routes.h" local="yes" import="no" module="no" objc="no">../../include/web_routes.h</includes>
    <includes id="wifi__manager_8h" name="wifi_manager.h" local="yes" import="no" module="no" objc="no">../wifi_manager.h</includes>
    <member kind="function">
      <type>void</type>
      <name>setupMainRoutes</name>
      <anchorfile>routes__main_8cpp.html</anchorfile>
      <anchor>a6a7cf6b6ba01d34721e1a47ea65af867</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleRoot</name>
      <anchorfile>routes__main_8cpp.html</anchorfile>
      <anchor>a8427468f39342a1990703e9f5ce7fd29</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>routes_ota.cpp</name>
    <path>src/web/</path>
    <filename>routes__ota_8cpp.html</filename>
    <includes id="jxct__config__vars_8h" name="jxct_config_vars.h" local="yes" import="no" module="no" objc="no">../../include/jxct_config_vars.h</includes>
    <includes id="jxct__constants_8h" name="jxct_constants.h" local="yes" import="no" module="no" objc="no">../../include/jxct_constants.h</includes>
    <includes id="jxct__strings_8h" name="jxct_strings.h" local="yes" import="no" module="no" objc="no">../../include/jxct_strings.h</includes>
    <includes id="jxct__ui__system_8h" name="jxct_ui_system.h" local="yes" import="no" module="no" objc="no">../../include/jxct_ui_system.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">../../include/logger.h</includes>
    <includes id="version_8h" name="version.h" local="yes" import="no" module="no" objc="no">../../include/version.h</includes>
    <includes id="web__routes_8h" name="web_routes.h" local="yes" import="no" module="no" objc="no">../../include/web_routes.h</includes>
    <includes id="wifi__manager_8h" name="wifi_manager.h" local="yes" import="no" module="no" objc="no">../wifi_manager.h</includes>
    <includes id="ota__manager_8h" name="ota_manager.h" local="yes" import="no" module="no" objc="no">ota_manager.h</includes>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>sendOtaStatusJson</name>
      <anchorfile>routes__ota_8cpp.html</anchorfile>
      <anchor>a5f391e260218b4827e75c958f389decc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>handleFirmwareUpload</name>
      <anchorfile>routes__ota_8cpp.html</anchorfile>
      <anchor>aa15bea881a160885a361e129a35c07e7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupOtaRoutes</name>
      <anchorfile>routes__ota_8cpp.html</anchorfile>
      <anchor>ae58a54419ee068a06deeb05bf1e771f4</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>routes_reports.cpp</name>
    <path>src/web/</path>
    <filename>routes__reports_8cpp.html</filename>
    <includes id="jxct__config__vars_8h" name="jxct_config_vars.h" local="yes" import="no" module="no" objc="no">../../include/jxct_config_vars.h</includes>
    <includes id="jxct__constants_8h" name="jxct_constants.h" local="yes" import="no" module="no" objc="no">../../include/jxct_constants.h</includes>
    <includes id="jxct__ui__system_8h" name="jxct_ui_system.h" local="yes" import="no" module="no" objc="no">../../include/jxct_ui_system.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">../../include/logger.h</includes>
    <includes id="web__routes_8h" name="web_routes.h" local="yes" import="no" module="no" objc="no">../../include/web_routes.h</includes>
    <includes id="wifi__manager_8h" name="wifi_manager.h" local="yes" import="no" module="no" objc="no">../wifi_manager.h</includes>
    <class kind="struct">TestSummary</class>
    <class kind="struct">TechnicalDebtMetrics</class>
    <member kind="function">
      <type>void</type>
      <name>setupReportsRoutes</name>
      <anchorfile>routes__reports_8cpp.html</anchorfile>
      <anchor>a1330f997c3d2a043a5ac4efc8f9906e7</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>routes_service.cpp</name>
    <path>src/web/</path>
    <filename>routes__service_8cpp.html</filename>
    <includes id="jxct__config__vars_8h" name="jxct_config_vars.h" local="yes" import="no" module="no" objc="no">../../include/jxct_config_vars.h</includes>
    <includes id="jxct__constants_8h" name="jxct_constants.h" local="yes" import="no" module="no" objc="no">../../include/jxct_constants.h</includes>
    <includes id="jxct__device__info_8h" name="jxct_device_info.h" local="yes" import="no" module="no" objc="no">../../include/jxct_device_info.h</includes>
    <includes id="jxct__format__utils_8h" name="jxct_format_utils.h" local="yes" import="no" module="no" objc="no">../../include/jxct_format_utils.h</includes>
    <includes id="jxct__strings_8h" name="jxct_strings.h" local="yes" import="no" module="no" objc="no">../../include/jxct_strings.h</includes>
    <includes id="jxct__ui__system_8h" name="jxct_ui_system.h" local="yes" import="no" module="no" objc="no">../../include/jxct_ui_system.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">../../include/logger.h</includes>
    <includes id="csrf__protection_8h" name="csrf_protection.h" local="yes" import="no" module="no" objc="no">../../include/web/csrf_protection.h</includes>
    <includes id="web__routes_8h" name="web_routes.h" local="yes" import="no" module="no" objc="no">../../include/web_routes.h</includes>
    <includes id="modbus__sensor_8h" name="modbus_sensor.h" local="yes" import="no" module="no" objc="no">../modbus_sensor.h</includes>
    <includes id="mqtt__client_8h" name="mqtt_client.h" local="yes" import="no" module="no" objc="no">../mqtt_client.h</includes>
    <includes id="thingspeak__client_8h" name="thingspeak_client.h" local="yes" import="no" module="no" objc="no">../thingspeak_client.h</includes>
    <includes id="wifi__manager_8h" name="wifi_manager.h" local="yes" import="no" module="no" objc="no">../wifi_manager.h</includes>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>sendHealthJson</name>
      <anchorfile>routes__service_8cpp.html</anchorfile>
      <anchor>a12d3ed3cb7af947fa5b68db68fa23ba6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>sendServiceStatusJson</name>
      <anchorfile>routes__service_8cpp.html</anchorfile>
      <anchor>a3d42f2d7785857ddcc08c615bddfd642</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static String</type>
      <name>formatUptime</name>
      <anchorfile>routes__service_8cpp.html</anchorfile>
      <anchor>abfa915ba3c1d9d59ce0a664b2a1ac7f2</anchor>
      <arglist>(unsigned long milliseconds)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupServiceRoutes</name>
      <anchorfile>routes__service_8cpp.html</anchorfile>
      <anchor>a32c74f47a23de058ee90a43af2dff066</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>web_templates.cpp</name>
    <path>src/web/</path>
    <filename>web__templates_8cpp.html</filename>
    <includes id="jxct__constants_8h" name="jxct_constants.h" local="yes" import="no" module="no" objc="no">../../include/jxct_constants.h</includes>
    <includes id="jxct__ui__system_8h" name="jxct_ui_system.h" local="yes" import="no" module="no" objc="no">../../include/jxct_ui_system.h</includes>
    <includes id="web__routes_8h" name="web_routes.h" local="yes" import="no" module="no" objc="no">../../include/web_routes.h</includes>
    <includes id="wifi__manager_8h" name="wifi_manager.h" local="yes" import="no" module="no" objc="no">../wifi_manager.h</includes>
    <member kind="function">
      <type>String</type>
      <name>generatePageHeader</name>
      <anchorfile>web__templates_8cpp.html</anchorfile>
      <anchor>adce6a7937046f635ece4ddca88d59fc6</anchor>
      <arglist>(const String &amp;titleText, const String &amp;iconText)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generatePageFooter</name>
      <anchorfile>web__templates_8cpp.html</anchorfile>
      <anchor>ad4f3a6e85baef9b2d3e2aec977c3a321</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generateBasePage</name>
      <anchorfile>web__templates_8cpp.html</anchorfile>
      <anchor>a6fceb01367cca8cb912d8a45661dc4c7</anchor>
      <arglist>(const String &amp;titleText, const String &amp;contentText, const String &amp;iconText)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generateErrorPage</name>
      <anchorfile>web__templates_8cpp.html</anchorfile>
      <anchor>a104303f7b44cf43e481a713261d52457</anchor>
      <arglist>(int errorCode, const String &amp;errorMessage)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generateSuccessPage</name>
      <anchorfile>web__templates_8cpp.html</anchorfile>
      <anchor>af42b5e3dc65eb3e396beb619d41733b4</anchor>
      <arglist>(const String &amp;titleText, const String &amp;messageText, const String &amp;redirectUrlText, int redirectDelaySeconds)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>generateApModeUnavailablePage</name>
      <anchorfile>web__templates_8cpp.html</anchorfile>
      <anchor>a7288aea19e0b4bd891ea87a13d14a521</anchor>
      <arglist>(const String &amp;titleText, const String &amp;iconText)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>wifi_manager.cpp</name>
    <path>src/</path>
    <filename>wifi__manager_8cpp.html</filename>
    <includes id="wifi__manager_8h" name="wifi_manager.h" local="yes" import="no" module="no" objc="no">wifi_manager.h</includes>
    <includes id="jxct__config__vars_8h" name="jxct_config_vars.h" local="yes" import="no" module="no" objc="no">jxct_config_vars.h</includes>
    <includes id="jxct__constants_8h" name="jxct_constants.h" local="yes" import="no" module="no" objc="no">jxct_constants.h</includes>
    <includes id="jxct__device__info_8h" name="jxct_device_info.h" local="yes" import="no" module="no" objc="no">jxct_device_info.h</includes>
    <includes id="jxct__format__utils_8h" name="jxct_format_utils.h" local="yes" import="no" module="no" objc="no">jxct_format_utils.h</includes>
    <includes id="jxct__ui__system_8h" name="jxct_ui_system.h" local="yes" import="no" module="no" objc="no">jxct_ui_system.h</includes>
    <includes id="logger_8h" name="logger.h" local="yes" import="no" module="no" objc="no">logger.h</includes>
    <includes id="modbus__sensor_8h" name="modbus_sensor.h" local="yes" import="no" module="no" objc="no">modbus_sensor.h</includes>
    <includes id="mqtt__client_8h" name="mqtt_client.h" local="yes" import="no" module="no" objc="no">mqtt_client.h</includes>
    <includes id="thingspeak__client_8h" name="thingspeak_client.h" local="yes" import="no" module="no" objc="no">thingspeak_client.h</includes>
    <includes id="csrf__protection_8h" name="csrf_protection.h" local="yes" import="no" module="no" objc="no">web/csrf_protection.h</includes>
    <includes id="web__routes_8h" name="web_routes.h" local="yes" import="no" module="no" objc="no">web_routes.h</includes>
    <member kind="enumeration">
      <type></type>
      <name>WifiConstants</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>a777d3a0d24c4d7d746f14df96bf298c3</anchor>
      <arglist></arglist>
      <enumvalue file="wifi__manager_8cpp.html" anchor="a777d3a0d24c4d7d746f14df96bf298c3a793251760dac40c0cdb51628526faf34">RESET_BUTTON_PIN</enumvalue>
      <enumvalue file="wifi__manager_8cpp.html" anchor="a777d3a0d24c4d7d746f14df96bf298c3a7f7590ded6640ce9b8ae09921cd6ebd6">WIFI_RECONNECT_INTERVAL</enumvalue>
      <enumvalue file="wifi__manager_8cpp.html" anchor="a777d3a0d24c4d7d746f14df96bf298c3a3e2ee68cf652cd628390f1229c4282ee">LED_FAST_BLINK_INTERVAL</enumvalue>
      <enumvalue file="wifi__manager_8cpp.html" anchor="a777d3a0d24c4d7d746f14df96bf298c3a2aa793152e07f106f8d9b46baa7de739">LED_SLOW_BLINK_INTERVAL</enumvalue>
      <enumvalue file="wifi__manager_8cpp.html" anchor="a777d3a0d24c4d7d746f14df96bf298c3a3331beb0a8148a1ac673dabf27b88549">WIFI_MODE_DELAY</enumvalue>
      <enumvalue file="wifi__manager_8cpp.html" anchor="a777d3a0d24c4d7d746f14df96bf298c3a9e25f8f4f826e474d6b00600b162751e">NTP_TIMEOUT_MS</enumvalue>
      <enumvalue file="wifi__manager_8cpp.html" anchor="a777d3a0d24c4d7d746f14df96bf298c3a452484feac01e8c37266a21d3d3ff64b">RESET_BUTTON_HOLD_TIME</enumvalue>
      <enumvalue file="wifi__manager_8cpp.html" anchor="a777d3a0d24c4d7d746f14df96bf298c3ab9993e0d7f6bf58161cb32c7baf74150">RESTART_DELAY_MS</enumvalue>
      <enumvalue file="wifi__manager_8cpp.html" anchor="a777d3a0d24c4d7d746f14df96bf298c3a5522e9d5fd9cf70194fc37fe9f77ff54">DNS_SERVER_PORT</enumvalue>
      <enumvalue file="wifi__manager_8cpp.html" anchor="a777d3a0d24c4d7d746f14df96bf298c3aa5a9710df8dc897f56c6456a006a5893">MAC_ADDRESS_BUFFER_SIZE</enumvalue>
    </member>
    <member kind="function">
      <type>WebServer</type>
      <name>webServer</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>a87bbeba035b82cbe7777c65b4e9fede4</anchor>
      <arglist>(DEFAULT_WEB_SERVER_PORT)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setLedOn</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>a3e4fd9a252cddb70e2b894b1bc6c0f5a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setLedOff</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>ab314091f4e2540ab48709ac656457472</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setLedBlink</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>a5f70031f46d0339f5c544c194dfe7ed7</anchor>
      <arglist>(unsigned long interval)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setLedFastBlink</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>ab4e589808cf702c83022456542ce6301</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>updateLed</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>aa43f68b6c12da400ebbda9664b030944</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>navHtml</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>ad30c0b8b33e60ccdb13f2e1cc08157c9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupWiFi</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>ad2a97dbe5f144cd247e3b15c6af7c767</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleWiFi</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>aef11d16b4d8e760671e3487b2106b6b2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>getApSsid</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>af857f35623b29612a3b4cc45dd6fff23</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>startAPMode</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>a9b3d409b1449092a0b4efaace9c666a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>startSTAMode</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>a0a1172dd684c9ed282dc5cb7e1ad1d7f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>checkResetButton</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>ac90eb7c31515447a075eee8f395ac02f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>restartESP</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>a40cec05f995ed2024366761fa28604a7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleStatus</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>a1e811785ccbaccda2e3e3457971720cb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupWebServer</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>ada2454c68167b49ab4f42817be2b4959</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>wifiConnected</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>ac542c876ea54085aa0173a9cd1e3251c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>WiFiMode</type>
      <name>currentWiFiMode</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>afd1ea40c3b78acfa354aed81da58e582</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>NTPClient *</type>
      <name>timeClient</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>ab4cd0eb2684ad54d1d0782f4ff75e07f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>WiFiUDP</type>
      <name>ntpUDP</name>
      <anchorfile>wifi__manager_8cpp.html</anchorfile>
      <anchor>a22f33e7e05df58bb6145bb6e543e232a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>wifi_manager.h</name>
    <path>src/</path>
    <filename>wifi__manager_8h.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>STATUS_LED_PIN</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>a089a71f836911c71b3f73fdd3b4b890b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>WiFiMode</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>aedfbbc69179e5329a61bb7578846c1b6</anchor>
      <arglist></arglist>
      <enumvalue file="wifi__manager_8h.html" anchor="aedfbbc69179e5329a61bb7578846c1b6a0fd3f8dd5edc33b28db1162e15e8fcbc">AP</enumvalue>
      <enumvalue file="wifi__manager_8h.html" anchor="aedfbbc69179e5329a61bb7578846c1b6a971ab16c6e59a04d070cb8d8da13418e">STA</enumvalue>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setLedOn</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>a3e4fd9a252cddb70e2b894b1bc6c0f5a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setLedOff</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>ab314091f4e2540ab48709ac656457472</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setLedBlink</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>a5f70031f46d0339f5c544c194dfe7ed7</anchor>
      <arglist>(unsigned long interval)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setLedFastBlink</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>ab4e589808cf702c83022456542ce6301</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>updateLed</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>aa43f68b6c12da400ebbda9664b030944</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupWebServer</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>ada2454c68167b49ab4f42817be2b4959</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setupWiFi</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>ad2a97dbe5f144cd247e3b15c6af7c767</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>handleWiFi</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>aef11d16b4d8e760671e3487b2106b6b2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>startAPMode</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>a9b3d409b1449092a0b4efaace9c666a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>startSTAMode</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>a0a1172dd684c9ed282dc5cb7e1ad1d7f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>checkResetButton</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>ac90eb7c31515447a075eee8f395ac02f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resetConfig</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>a6fa5204bcafe1db397da4a87b1690061</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>restartESP</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>a40cec05f995ed2024366761fa28604a7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>parseAndApplyConfig</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>a62a0f48d52173eca4f1a069c96840851</anchor>
      <arglist>(const String &amp;jsonContent, String &amp;error)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>navHtml</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>ad30c0b8b33e60ccdb13f2e1cc08157c9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>getApSsid</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>af857f35623b29612a3b4cc45dd6fff23</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>wifiConnected</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>ac542c876ea54085aa0173a9cd1e3251c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>WiFiMode</type>
      <name>currentWiFiMode</name>
      <anchorfile>wifi__manager_8h.html</anchorfile>
      <anchor>afd1ea40c3b78acfa354aed81da58e582</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ArchieCoefficients</name>
    <filename>structArchieCoefficients.html</filename>
    <member kind="function">
      <type></type>
      <name>ArchieCoefficients</name>
      <anchorfile>structArchieCoefficients.html</anchorfile>
      <anchor>aff4a33eb6716ad8b9189674cfb7beea6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ArchieCoefficients</name>
      <anchorfile>structArchieCoefficients.html</anchorfile>
      <anchor>a18fd767d9a2a43e085e4d58c39143f1d</anchor>
      <arglist>(float cementation, float saturation, float porosity)</arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>m</name>
      <anchorfile>structArchieCoefficients.html</anchorfile>
      <anchor>a8a7dd1460f557fcba78a18a2ce4c1882</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>n</name>
      <anchorfile>structArchieCoefficients.html</anchorfile>
      <anchor>a13e0d744c0b089788d8790905ee2985a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>a</name>
      <anchorfile>structArchieCoefficients.html</anchorfile>
      <anchor>a320c05e5003f55503a3c8f336730259e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>BasicSensorAdapter</name>
    <filename>classBasicSensorAdapter.html</filename>
    <base>ISensor</base>
    <member kind="typedef">
      <type>void(*)()</type>
      <name>InitFn</name>
      <anchorfile>classBasicSensorAdapter.html</anchorfile>
      <anchor>adc4772b3cd6b0bbc516e3c1a5076bf93</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void(*)()</type>
      <name>AcquireFn</name>
      <anchorfile>classBasicSensorAdapter.html</anchorfile>
      <anchor>a7209e637c892c9013df462380c97eb96</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>BasicSensorAdapter</name>
      <anchorfile>classBasicSensorAdapter.html</anchorfile>
      <anchor>a773a58c9867d8bb60dbad596505d10ed</anchor>
      <arglist>(const char *n, InitFn initFn, AcquireFn acquireFn, SensorData *shared)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>begin</name>
      <anchorfile>classBasicSensorAdapter.html</anchorfile>
      <anchor>aae826d03be1e4a98ebefe19e32839636</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>read</name>
      <anchorfile>classBasicSensorAdapter.html</anchorfile>
      <anchor>ab2dd0a95116651b6f61f738d28132d16</anchor>
      <arglist>(SensorData &amp;out) override</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>name</name>
      <anchorfile>classBasicSensorAdapter.html</anchorfile>
      <anchor>a2a494f4bca421e89a5ac8de2a9b79d49</anchor>
      <arglist>() const override</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CropCompensationParams::Builder</name>
    <filename>structCropCompensationParams_1_1Builder.html</filename>
    <member kind="function">
      <type>Builder &amp;</type>
      <name>setRawValue</name>
      <anchorfile>structCropCompensationParams_1_1Builder.html</anchorfile>
      <anchor>ad2ed34830af6e32ae257efc33989591d</anchor>
      <arglist>(float value)</arglist>
    </member>
    <member kind="function">
      <type>Builder &amp;</type>
      <name>setTemperature</name>
      <anchorfile>structCropCompensationParams_1_1Builder.html</anchorfile>
      <anchor>a28dedafa9b5dd1cdb3fba7921732b66b</anchor>
      <arglist>(float temp)</arglist>
    </member>
    <member kind="function">
      <type>Builder &amp;</type>
      <name>setMoisture</name>
      <anchorfile>structCropCompensationParams_1_1Builder.html</anchorfile>
      <anchor>a65d40006c64163e259839e1b3583b10d</anchor>
      <arglist>(float moist)</arglist>
    </member>
    <member kind="function">
      <type>CropCompensationParams</type>
      <name>build</name>
      <anchorfile>structCropCompensationParams_1_1Builder.html</anchorfile>
      <anchor>a2757f47824a5c37868e1c7c0ae7f8ec2</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>rawValue</name>
      <anchorfile>structCropCompensationParams_1_1Builder.html</anchorfile>
      <anchor>a178d8b840f5ed2cc1d3b75c437052ac1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>temperature</name>
      <anchorfile>structCropCompensationParams_1_1Builder.html</anchorfile>
      <anchor>abce258c651950a0c3be602761f74bd2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>moisture</name>
      <anchorfile>structCropCompensationParams_1_1Builder.html</anchorfile>
      <anchor>a213e12c4d0b9eb564a3c4f415bee36e4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CropECCompensationParams::Builder</name>
    <filename>structCropECCompensationParams_1_1Builder.html</filename>
    <member kind="function">
      <type>Builder &amp;</type>
      <name>setRawValue</name>
      <anchorfile>structCropECCompensationParams_1_1Builder.html</anchorfile>
      <anchor>a1ed86f5943bd2d6d9224f59f7a860155</anchor>
      <arglist>(float value)</arglist>
    </member>
    <member kind="function">
      <type>Builder &amp;</type>
      <name>setTemperature</name>
      <anchorfile>structCropECCompensationParams_1_1Builder.html</anchorfile>
      <anchor>abc4bf117bd0ec5a9dac656581fe88d76</anchor>
      <arglist>(float temp)</arglist>
    </member>
    <member kind="function">
      <type>CropECCompensationParams</type>
      <name>build</name>
      <anchorfile>structCropECCompensationParams_1_1Builder.html</anchorfile>
      <anchor>a5a7037a51ecd48d56013a0000e76b9fa</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>rawValue</name>
      <anchorfile>structCropECCompensationParams_1_1Builder.html</anchorfile>
      <anchor>aa3f259033a3147a2eaf13f7ea8122f98</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>temperature</name>
      <anchorfile>structCropECCompensationParams_1_1Builder.html</anchorfile>
      <anchor>aba536414d77234adde8a5e0b35a9e8f2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>HttpRequest::Builder</name>
    <filename>structHttpRequest_1_1Builder.html</filename>
    <member kind="function">
      <type>Builder &amp;</type>
      <name>method</name>
      <anchorfile>structHttpRequest_1_1Builder.html</anchorfile>
      <anchor>ae21b4adf0754993a3721cabec5d38c4b</anchor>
      <arglist>(const String &amp;method)</arglist>
    </member>
    <member kind="function">
      <type>Builder &amp;</type>
      <name>uri</name>
      <anchorfile>structHttpRequest_1_1Builder.html</anchorfile>
      <anchor>a804ed3dfd61e1ed67ce8224a296f332e</anchor>
      <arglist>(const String &amp;uri)</arglist>
    </member>
    <member kind="function">
      <type>Builder &amp;</type>
      <name>clientIP</name>
      <anchorfile>structHttpRequest_1_1Builder.html</anchorfile>
      <anchor>afbe3435b884f6e21a6f6eb7143ff9985</anchor>
      <arglist>(const String &amp;clientIP)</arglist>
    </member>
    <member kind="function">
      <type>HttpRequest</type>
      <name>build</name>
      <anchorfile>structHttpRequest_1_1Builder.html</anchorfile>
      <anchor>ab612743c44ab90765612a9e8b216bca5</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>methodValue</name>
      <anchorfile>structHttpRequest_1_1Builder.html</anchorfile>
      <anchor>ae12d0a0496458a374620aaeed0b9af76</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>uriValue</name>
      <anchorfile>structHttpRequest_1_1Builder.html</anchorfile>
      <anchor>a81717d18286235a9d49a0ddec9f031ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>clientIPValue</name>
      <anchorfile>structHttpRequest_1_1Builder.html</anchorfile>
      <anchor>aa5e66e649344642ad35e732ec526e813</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RecommendationParams::Builder</name>
    <filename>structRecommendationParams_1_1Builder.html</filename>
    <member kind="function">
      <type>Builder &amp;</type>
      <name>data</name>
      <anchorfile>structRecommendationParams_1_1Builder.html</anchorfile>
      <anchor>ae1a10b89100e48459393b46f3f297f16</anchor>
      <arglist>(const SensorData &amp;sensorDataValue)</arglist>
    </member>
    <member kind="function">
      <type>Builder &amp;</type>
      <name>cropType</name>
      <anchorfile>structRecommendationParams_1_1Builder.html</anchorfile>
      <anchor>af7574d30c508616648f234904e1bf9c0</anchor>
      <arglist>(const String &amp;cropTypeValue)</arglist>
    </member>
    <member kind="function">
      <type>Builder &amp;</type>
      <name>growingType</name>
      <anchorfile>structRecommendationParams_1_1Builder.html</anchorfile>
      <anchor>aa4bf8e988cde3c2445fc73b4d2e01b24</anchor>
      <arglist>(const String &amp;growingTypeValue)</arglist>
    </member>
    <member kind="function">
      <type>Builder &amp;</type>
      <name>season</name>
      <anchorfile>structRecommendationParams_1_1Builder.html</anchorfile>
      <anchor>ab29cff8f5bac99e4f0857b6f5260d5cc</anchor>
      <arglist>(const String &amp;seasonValue)</arglist>
    </member>
    <member kind="function">
      <type>Builder &amp;</type>
      <name>soilType</name>
      <anchorfile>structRecommendationParams_1_1Builder.html</anchorfile>
      <anchor>a865f88da5f1ba92cab64fd552e5e4686</anchor>
      <arglist>(const String &amp;soilTypeValue)</arglist>
    </member>
    <member kind="function">
      <type>RecommendationParams</type>
      <name>build</name>
      <anchorfile>structRecommendationParams_1_1Builder.html</anchorfile>
      <anchor>a679a2955da5064f12bb8643e3dc044cf</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="variable">
      <type>SensorData</type>
      <name>sensorData</name>
      <anchorfile>structRecommendationParams_1_1Builder.html</anchorfile>
      <anchor>a63c5499ea94bcf849e43a2625f389489</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>crop</name>
      <anchorfile>structRecommendationParams_1_1Builder.html</anchorfile>
      <anchor>a9b3e2e2b8cf27c7005c63a3a4ad020d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>growing</name>
      <anchorfile>structRecommendationParams_1_1Builder.html</anchorfile>
      <anchor>ae9d8907f70f9e21161e7e2ad847255a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>seasonType</name>
      <anchorfile>structRecommendationParams_1_1Builder.html</anchorfile>
      <anchor>a54180ff9f7cb0d9ac3000a5f968d5b2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>soil</name>
      <anchorfile>structRecommendationParams_1_1Builder.html</anchorfile>
      <anchor>a526e73d889a44b2364d45a5cde6f825e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ValidationRange::Builder</name>
    <filename>structValidationRange_1_1Builder.html</filename>
    <member kind="function">
      <type>Builder &amp;</type>
      <name>minValue</name>
      <anchorfile>structValidationRange_1_1Builder.html</anchorfile>
      <anchor>aaee0ba2f1a5bf8b7feb2f31932a0e5b0</anchor>
      <arglist>(int minValue)</arglist>
    </member>
    <member kind="function">
      <type>Builder &amp;</type>
      <name>maxValue</name>
      <anchorfile>structValidationRange_1_1Builder.html</anchorfile>
      <anchor>a8ad623ec82dc6ed14f039ff972b9aff6</anchor>
      <arglist>(int maxValue)</arglist>
    </member>
    <member kind="function">
      <type>ValidationRange</type>
      <name>build</name>
      <anchorfile>structValidationRange_1_1Builder.html</anchorfile>
      <anchor>af859f4e36100b8f3c935bee6b7f036ad</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>min</name>
      <anchorfile>structValidationRange_1_1Builder.html</anchorfile>
      <anchor>a4fce216e0746101c7ef338f1b8b4940a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>max</name>
      <anchorfile>structValidationRange_1_1Builder.html</anchorfile>
      <anchor>a8f87ea4f39372358e2fa5ece0751c2fb</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ButtonConfig</name>
    <filename>structButtonConfig.html</filename>
    <member kind="variable">
      <type>const char *</type>
      <name>icon</name>
      <anchorfile>structButtonConfig.html</anchorfile>
      <anchor>a98cd54b05bb462e769be8b4632754ef3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>text</name>
      <anchorfile>structButtonConfig.html</anchorfile>
      <anchor>a5b33c0d3edf46628fdff0dfef319dc6e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>action</name>
      <anchorfile>structButtonConfig.html</anchorfile>
      <anchor>a4259b543985728e6c0b961f015a21cfd</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CalibrationEntry</name>
    <filename>structCalibrationEntry.html</filename>
    <member kind="variable">
      <type>float</type>
      <name>raw</name>
      <anchorfile>structCalibrationEntry.html</anchorfile>
      <anchor>abf1254be84455b3cc41e4bb9e1e9dd16</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>corrected</name>
      <anchorfile>structCalibrationEntry.html</anchorfile>
      <anchor>ae6c3c77310b260c6d0e5c5624ab507fa</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CalibrationPoint</name>
    <filename>structCalibrationPoint.html</filename>
    <member kind="function">
      <type></type>
      <name>CalibrationPoint</name>
      <anchorfile>structCalibrationPoint.html</anchorfile>
      <anchor>ada17fc99267680776ee10aeed99fe2dc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>CalibrationPoint</name>
      <anchorfile>structCalibrationPoint.html</anchorfile>
      <anchor>a2f71984d97146d8b9ea5e5862b5d8993</anchor>
      <arglist>(float raw, float reference)</arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>rawValue</name>
      <anchorfile>structCalibrationPoint.html</anchorfile>
      <anchor>a99ff5dab554d4145430b07c26ca16050</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>referenceValue</name>
      <anchorfile>structCalibrationPoint.html</anchorfile>
      <anchor>a5aaefc6d40fcd8b61be5b9b26e8c88d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>expected</name>
      <anchorfile>structCalibrationPoint.html</anchorfile>
      <anchor>afaa050b771582e286953fe2527dc27f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>measured</name>
      <anchorfile>structCalibrationPoint.html</anchorfile>
      <anchor>a1681dd4a208700268b870076c9e2e24a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CalibrationTable</name>
    <filename>structCalibrationTable.html</filename>
    <member kind="function">
      <type></type>
      <name>CalibrationTable</name>
      <anchorfile>structCalibrationTable.html</anchorfile>
      <anchor>a72e2c9012e0ec51bf5fa2f8cb03b8adb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; CalibrationPoint &gt;</type>
      <name>temperaturePoints</name>
      <anchorfile>structCalibrationTable.html</anchorfile>
      <anchor>aaef86f866eb89ac4d3d810b473812851</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; CalibrationPoint &gt;</type>
      <name>humidityPoints</name>
      <anchorfile>structCalibrationTable.html</anchorfile>
      <anchor>ae83d5b35d77ed007f63d5ba7bafd89fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; CalibrationPoint &gt;</type>
      <name>ecPoints</name>
      <anchorfile>structCalibrationTable.html</anchorfile>
      <anchor>a1f77c3ba4b807a6841d8cc57b1009f3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; CalibrationPoint &gt;</type>
      <name>phPoints</name>
      <anchorfile>structCalibrationTable.html</anchorfile>
      <anchor>acc74a70992cd7c3c28e4b86a98cfee36</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; CalibrationPoint &gt;</type>
      <name>nitrogenPoints</name>
      <anchorfile>structCalibrationTable.html</anchorfile>
      <anchor>a47f16e0b871807408feaf9e94e399506</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; CalibrationPoint &gt;</type>
      <name>phosphorusPoints</name>
      <anchorfile>structCalibrationTable.html</anchorfile>
      <anchor>a509c8bd6f1825b6b0d25a3c9b90094ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; CalibrationPoint &gt;</type>
      <name>potassiumPoints</name>
      <anchorfile>structCalibrationTable.html</anchorfile>
      <anchor>a760063dc126716d2a91a350521ca93f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>isValid</name>
      <anchorfile>structCalibrationTable.html</anchorfile>
      <anchor>afbccd104eaa6d9318c20983c69a27894</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ConfigData</name>
    <filename>structConfigData.html</filename>
    <member kind="variable">
      <type>String</type>
      <name>ssid</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>ae280e01c917c44f8fdaea8ea803b2398</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>password</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>a16d6eaf819e4a207fcf52b3038347231</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>mqttEnabled</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>a9680e7b026b5b2ca4fba7725a14b269a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>mqttServer</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>a6f1dfd087cc2fb57f07e9c906c21318e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>mqttPort</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>aa3a6c46c326325f44c5cf551094e4012</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>thingSpeakEnabled</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>a0297efbded709e911c6a26ec58b4d16f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>thingSpeakAPIKey</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>a38d97f1bfec0f08c3b4ffd4177866660</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned long</type>
      <name>sensorReadInterval</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>ad39ef7e66372933b6db3bc612b36e149</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned long</type>
      <name>mqttPublishInterval</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>a4cd4bdfc03b67b1a6d0a9c53f3d51148</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned long</type>
      <name>thingspeakInterval</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>adb6ae912fb0f894f37e86eaeaf964800</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned long</type>
      <name>ntpUpdateInterval</name>
      <anchorfile>structConfigData.html</anchorfile>
      <anchor>a2d2106d586c94e91e3d797d77deacdaf</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ConfigValidationResult</name>
    <filename>structConfigValidationResult.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>isValid</name>
      <anchorfile>structConfigValidationResult.html</anchorfile>
      <anchor>ae1533b894732e63a2322865fba65c499</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; ValidationError &gt;</type>
      <name>errors</name>
      <anchorfile>structConfigValidationResult.html</anchorfile>
      <anchor>adfe9c7666685cd227d3a2c4d0e10efe1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CropCompensationParams</name>
    <filename>structCropCompensationParams.html</filename>
    <class kind="struct">CropCompensationParams::Builder</class>
    <member kind="function" static="yes">
      <type>static Builder</type>
      <name>builder</name>
      <anchorfile>structCropCompensationParams.html</anchorfile>
      <anchor>a1013ccb29597ab4e4b16ddd8b97bc8f2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>rawValue</name>
      <anchorfile>structCropCompensationParams.html</anchorfile>
      <anchor>ac68a8ddc9b05ab24d142722935e58ddb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>temperature</name>
      <anchorfile>structCropCompensationParams.html</anchorfile>
      <anchor>a30a5f1439ac6104d11563c1525154a9d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>moisture</name>
      <anchorfile>structCropCompensationParams.html</anchorfile>
      <anchor>ab951a2533badafb2f1a1c5f139b04962</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CropConfig</name>
    <filename>structCropConfig.html</filename>
    <member kind="function">
      <type></type>
      <name>CropConfig</name>
      <anchorfile>structCropConfig.html</anchorfile>
      <anchor>a143b4c57d738c2ff0f6beac940a7eb30</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>CropConfig</name>
      <anchorfile>structCropConfig.html</anchorfile>
      <anchor>a8dc22a79055b2d188599219d13b6da63</anchor>
      <arglist>(float t, float h, float e, float p, float n, float ph, float k)</arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>temperature</name>
      <anchorfile>structCropConfig.html</anchorfile>
      <anchor>a6396b34ee06cddfafeb51a6fe8c6673b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>humidity</name>
      <anchorfile>structCropConfig.html</anchorfile>
      <anchor>a14d0adaf211511dade7d561244030dad</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>ec</name>
      <anchorfile>structCropConfig.html</anchorfile>
      <anchor>adeef15ae99ba41875689af46d6259c2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>ph</name>
      <anchorfile>structCropConfig.html</anchorfile>
      <anchor>af5b41b0e1f835a14681f201462c2099f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>nitrogen</name>
      <anchorfile>structCropConfig.html</anchorfile>
      <anchor>ad320c47982971ef8abbb919252042203</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>phosphorus</name>
      <anchorfile>structCropConfig.html</anchorfile>
      <anchor>ad8bc80ce5bb46ac63ccce6220a1d58eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>potassium</name>
      <anchorfile>structCropConfig.html</anchorfile>
      <anchor>a00cc52bc3426a4dda6749145540489c1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>CropECCompensationParams</name>
    <filename>structCropECCompensationParams.html</filename>
    <class kind="struct">CropECCompensationParams::Builder</class>
    <member kind="function" static="yes">
      <type>static Builder</type>
      <name>builder</name>
      <anchorfile>structCropECCompensationParams.html</anchorfile>
      <anchor>af7897922930b8933666e3262e1ce7aed</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>rawValue</name>
      <anchorfile>structCropECCompensationParams.html</anchorfile>
      <anchor>aaea7291ec52df53ad5fcd89399c162f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>temperature</name>
      <anchorfile>structCropECCompensationParams.html</anchorfile>
      <anchor>abf4c3895eb5688c173f8f86c0edc4d96</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>CropRecommendationEngine</name>
    <filename>classCropRecommendationEngine.html</filename>
    <base>ICropRecommendationEngine</base>
    <member kind="function">
      <type></type>
      <name>CropRecommendationEngine</name>
      <anchorfile>classCropRecommendationEngine.html</anchorfile>
      <anchor>ae449fe9732b3cef2994a6e0097546c3f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>RecommendationResult</type>
      <name>generateRecommendation</name>
      <anchorfile>classCropRecommendationEngine.html</anchorfile>
      <anchor>a9ed0637318a2c91c089d9d483e79c9ee</anchor>
      <arglist>(const SensorData &amp;data, const String &amp;cropType, const String &amp;growingType=&quot;soil&quot;, const String &amp;season=&quot;spring&quot;, const String &amp;soilType=&quot;loam&quot;)</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; String &gt;</type>
      <name>getAvailableCrops</name>
      <anchorfile>classCropRecommendationEngine.html</anchorfile>
      <anchor>a7354b5a34985337c3f8f7020970d3ec3</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>CropConfig</type>
      <name>getCropConfig</name>
      <anchorfile>classCropRecommendationEngine.html</anchorfile>
      <anchor>a5d5577eb20020794535ce137e4d6008d</anchor>
      <arglist>(const String &amp;cropType) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>validateSensorData</name>
      <anchorfile>classCropRecommendationEngine.html</anchorfile>
      <anchor>aa747684ad3c6fe0d29fe19ae82a401ec</anchor>
      <arglist>(const SensorData &amp;data) const</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>getCropScientificInfo</name>
      <anchorfile>classCropRecommendationEngine.html</anchorfile>
      <anchor>a04df3f1f80fc2f250942b5d671168409</anchor>
      <arglist>(const String &amp;cropType) const</arglist>
    </member>
    <member kind="function">
      <type>RecValues</type>
      <name>computeRecommendations</name>
      <anchorfile>classCropRecommendationEngine.html</anchorfile>
      <anchor>a0b8e282e77bd12e9d6efb5e95f6d63d9</anchor>
      <arglist>(const String &amp;cropId, const SoilProfile &amp;soilProfile, const EnvironmentType &amp;envType) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>applySeasonalCorrection</name>
      <anchorfile>classCropRecommendationEngine.html</anchorfile>
      <anchor>a80d5dcf4a5b65a4540bd5999322fbd3d</anchor>
      <arglist>(RecValues &amp;rec, Season season, bool isGreenhouse) override</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ECCalibration</name>
    <filename>structECCalibration.html</filename>
    <member kind="variable">
      <type>std::vector&lt; CalibrationPoint &gt;</type>
      <name>points</name>
      <anchorfile>structECCalibration.html</anchorfile>
      <anchor>a274455be59637dc9bf633a708fa6b961</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>coefficient_c</name>
      <anchorfile>structECCalibration.html</anchorfile>
      <anchor>a53f0ea4bb84111351ba65b0d837a8340</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>coefficient_d</name>
      <anchorfile>structECCalibration.html</anchorfile>
      <anchor>a05449a38b6b5a1d7308efe503d83ab06</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>r_squared</name>
      <anchorfile>structECCalibration.html</anchorfile>
      <anchor>a1c822cb2f7b5a5f638735e98acd05f9a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>is_valid</name>
      <anchorfile>structECCalibration.html</anchorfile>
      <anchor>a915577341a84da8154c4f625792c4f42</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>AdvancedFilters::ECFilterState</name>
    <filename>structAdvancedFilters_1_1ECFilterState.html</filename>
    <member kind="function">
      <type></type>
      <name>ECFilterState</name>
      <anchorfile>structAdvancedFilters_1_1ECFilterState.html</anchorfile>
      <anchor>a1f38826969257185fc95831128be4f6a</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="variable">
      <type>std::array&lt; float, 10 &gt;</type>
      <name>recent_values</name>
      <anchorfile>structAdvancedFilters_1_1ECFilterState.html</anchorfile>
      <anchor>a88be821cddd6506790327ac1629ae4df</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>index</name>
      <anchorfile>structAdvancedFilters_1_1ECFilterState.html</anchorfile>
      <anchor>af8d9ba069521450c74867d2b99f38281</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>filled</name>
      <anchorfile>structAdvancedFilters_1_1ECFilterState.html</anchorfile>
      <anchor>ad8cdb74d930e392ca7058eaac9be9ac8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>baseline</name>
      <anchorfile>structAdvancedFilters_1_1ECFilterState.html</anchorfile>
      <anchor>a7a17dab68535b82eb51a2ae26ba981d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>last_spike_time</name>
      <anchorfile>structAdvancedFilters_1_1ECFilterState.html</anchorfile>
      <anchor>a79650472056e599055ec521468b3824b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>spike_count</name>
      <anchorfile>structAdvancedFilters_1_1ECFilterState.html</anchorfile>
      <anchor>aff100ff50c4f60183f37b85cf3760b11</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>baseline_valid</name>
      <anchorfile>structAdvancedFilters_1_1ECFilterState.html</anchorfile>
      <anchor>a060bf7eb99f969a67e8d2eb4c86be338</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>EnvironmentalConditions</name>
    <filename>structEnvironmentalConditions.html</filename>
    <member kind="variable">
      <type>float</type>
      <name>temperature</name>
      <anchorfile>structEnvironmentalConditions.html</anchorfile>
      <anchor>a7f842daec5328d50fe72f3e60b7eeda3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>moisture</name>
      <anchorfile>structEnvironmentalConditions.html</anchorfile>
      <anchor>a79eb8767ba75840b3ffe3048abd7adb4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>AdvancedFilters::ExponentialSmoothingState</name>
    <filename>structAdvancedFilters_1_1ExponentialSmoothingState.html</filename>
    <member kind="variable">
      <type>float</type>
      <name>smoothed_value</name>
      <anchorfile>structAdvancedFilters_1_1ExponentialSmoothingState.html</anchorfile>
      <anchor>a1fc7d8e8e5cd112f22ce230ffabf71b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>initialized</name>
      <anchorfile>structAdvancedFilters_1_1ExponentialSmoothingState.html</anchorfile>
      <anchor>a9a303cb358abfa7be34b3dd934a98dc5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>FakeSensorAdapter</name>
    <filename>classFakeSensorAdapter.html</filename>
    <base>BasicSensorAdapter</base>
    <member kind="function">
      <type></type>
      <name>FakeSensorAdapter</name>
      <anchorfile>classFakeSensorAdapter.html</anchorfile>
      <anchor>af18b89449d9a289d773170c66787bd23</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>HttpRequest</name>
    <filename>structHttpRequest.html</filename>
    <class kind="struct">HttpRequest::Builder</class>
    <member kind="function" static="yes">
      <type>static Builder</type>
      <name>builder</name>
      <anchorfile>structHttpRequest.html</anchorfile>
      <anchor>a99a3ecf43e7345d152752fbcb0f382db</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>method</name>
      <anchorfile>structHttpRequest.html</anchorfile>
      <anchor>a2d133650f24c6ba2407c8c541bdf2f22</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>uri</name>
      <anchorfile>structHttpRequest.html</anchorfile>
      <anchor>a6521bd8a35cbd5d4e41aae451192aa3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>clientIP</name>
      <anchorfile>structHttpRequest.html</anchorfile>
      <anchor>a1f8370d44f27dc7249a54452b062f6f2</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ICropRecommendationEngine</name>
    <filename>classICropRecommendationEngine.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~ICropRecommendationEngine</name>
      <anchorfile>classICropRecommendationEngine.html</anchorfile>
      <anchor>a3f2d1e6c98a2c1c455316850d405e72d</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual RecValues</type>
      <name>computeRecommendations</name>
      <anchorfile>classICropRecommendationEngine.html</anchorfile>
      <anchor>a268ebdf97416ec91340a461316fc9b2e</anchor>
      <arglist>(const String &amp;cropId, const SoilProfile &amp;soilProfile, const EnvironmentType &amp;envType)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>applySeasonalCorrection</name>
      <anchorfile>classICropRecommendationEngine.html</anchorfile>
      <anchor>a8fb69ec5ecc8eee64d51ce052cfaf141</anchor>
      <arglist>(RecValues &amp;rec, Season season, bool isGreenhouse)=0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>IMqttConnectionManager</name>
    <filename>classIMqttConnectionManager.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~IMqttConnectionManager</name>
      <anchorfile>classIMqttConnectionManager.html</anchorfile>
      <anchor>ac34c55f918877f1052cab55649320505</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>connect</name>
      <anchorfile>classIMqttConnectionManager.html</anchorfile>
      <anchor>a7a7da8bed03171ad2d42b04fa3df3e62</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>disconnect</name>
      <anchorfile>classIMqttConnectionManager.html</anchorfile>
      <anchor>a5a8d84a9ccc793693200adc5d3df538b</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>isConnected</name>
      <anchorfile>classIMqttConnectionManager.html</anchorfile>
      <anchor>af873b5975225e447d660d94a2725b31d</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>publish</name>
      <anchorfile>classIMqttConnectionManager.html</anchorfile>
      <anchor>a52cebd576ce32690352b0db6412cfe53</anchor>
      <arglist>(const String &amp;topic, const String &amp;payload, bool retained=false)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>subscribe</name>
      <anchorfile>classIMqttConnectionManager.html</anchorfile>
      <anchor>a73fb7a2fd7c8f4b7c336787ab028c7d8</anchor>
      <arglist>(const String &amp;topic)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>unsubscribe</name>
      <anchorfile>classIMqttConnectionManager.html</anchorfile>
      <anchor>a40205bd08192f7807f0a6b5211026017</anchor>
      <arglist>(const String &amp;topic)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>loop</name>
      <anchorfile>classIMqttConnectionManager.html</anchorfile>
      <anchor>aef96945d9dc9e7839b5f6b7ed5c284b2</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual String</type>
      <name>getLastError</name>
      <anchorfile>classIMqttConnectionManager.html</anchorfile>
      <anchor>a89a5a5cd471b5aa27e570c1b8fb6c013</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>setCallback</name>
      <anchorfile>classIMqttConnectionManager.html</anchorfile>
      <anchor>a744e680cb46ae776262eddbe277e84ce</anchor>
      <arglist>(std::function&lt; void(char *, uint8_t *, unsigned int)&gt; callback)=0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ISensor</name>
    <filename>classISensor.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~ISensor</name>
      <anchorfile>classISensor.html</anchorfile>
      <anchor>adbd5b40ebbf9a78b9845d22dddfac351</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>begin</name>
      <anchorfile>classISensor.html</anchorfile>
      <anchor>ad1404839a8965f45f6c2659dcb4333ba</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>read</name>
      <anchorfile>classISensor.html</anchorfile>
      <anchor>a5c622dbc4e674eda5b7ab05af7d14209</anchor>
      <arglist>(SensorData &amp;out)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual const char *</type>
      <name>name</name>
      <anchorfile>classISensor.html</anchorfile>
      <anchor>a552673f909141df52c9ea55e3be931e8</anchor>
      <arglist>() const =0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ISensorCalibrationService</name>
    <filename>classISensorCalibrationService.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~ISensorCalibrationService</name>
      <anchorfile>classISensorCalibrationService.html</anchorfile>
      <anchor>aed593e610cedec893fe02d1f0f293f07</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>applyCalibration</name>
      <anchorfile>classISensorCalibrationService.html</anchorfile>
      <anchor>ae024a7b3fd62fcbfa0310bd9ca836b34</anchor>
      <arglist>(SensorData &amp;data, SoilProfile profile)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual float</type>
      <name>applySingleCalibration</name>
      <anchorfile>classISensorCalibrationService.html</anchorfile>
      <anchor>aad01984f8f28237c4b768309eeed64c5</anchor>
      <arglist>(float rawValue, SoilProfile profile)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>validateCalibrationData</name>
      <anchorfile>classISensorCalibrationService.html</anchorfile>
      <anchor>a558e41ac1cab3570d42eb481373c661d</anchor>
      <arglist>(const SensorData &amp;data)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>loadCalibrationTable</name>
      <anchorfile>classISensorCalibrationService.html</anchorfile>
      <anchor>adfa09f462bed6593bea8164ef669aecc</anchor>
      <arglist>(const String &amp;csvData, SoilProfile profile)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>hasCalibrationTable</name>
      <anchorfile>classISensorCalibrationService.html</anchorfile>
      <anchor>af9876a3e5776debf1381b55c44ab6966</anchor>
      <arglist>(SoilProfile profile) const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>clearCalibrationTable</name>
      <anchorfile>classISensorCalibrationService.html</anchorfile>
      <anchor>ad43730890ab002d7aa5140a696485773</anchor>
      <arglist>(SoilProfile profile)=0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ISensorCompensationService</name>
    <filename>classISensorCompensationService.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~ISensorCompensationService</name>
      <anchorfile>classISensorCompensationService.html</anchorfile>
      <anchor>a152eb9ee78f5a5713f5e4a50e03f2e48</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>applyCompensation</name>
      <anchorfile>classISensorCompensationService.html</anchorfile>
      <anchor>a240c7f64bcde5b067b56e3f3cebd8d1b</anchor>
      <arglist>(SensorData &amp;data, SoilType soilType)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual float</type>
      <name>correctEC</name>
      <anchorfile>classISensorCompensationService.html</anchorfile>
      <anchor>acd5a3fa336f1f2cbe6c724433e5c079c</anchor>
      <arglist>(float ec25, SoilType soilType, float temperature, float humidity)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual float</type>
      <name>correctPH</name>
      <anchorfile>classISensorCompensationService.html</anchorfile>
      <anchor>a560934461631c9b3eae8ea89d7dbe623</anchor>
      <arglist>(float temperature, float phRaw)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>correctNPK</name>
      <anchorfile>classISensorCompensationService.html</anchorfile>
      <anchor>a4045e34fc5802a2b555dcaf27830fcf7</anchor>
      <arglist>(float temperature, float humidity, SoilType soilType, NPKReferences &amp;npk)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual float</type>
      <name>getArchieCoefficient</name>
      <anchorfile>classISensorCompensationService.html</anchorfile>
      <anchor>aa9811ec62ff94f4750d0c3d7e432f985</anchor>
      <arglist>(SoilType soilType) const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual float</type>
      <name>getPorosity</name>
      <anchorfile>classISensorCompensationService.html</anchorfile>
      <anchor>ac98e5cb70e2fc67cb5e58ddbcafd99f1</anchor>
      <arglist>(SoilType soilType) const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>validateCompensationInputs</name>
      <anchorfile>classISensorCompensationService.html</anchorfile>
      <anchor>acb84f18a0e3d9a1e1571c1695d78ccee</anchor>
      <arglist>(SoilType soilType, float humidity, float temperature) const =0</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>AdvancedFilters::KalmanFilter</name>
    <filename>structAdvancedFilters_1_1KalmanFilter.html</filename>
    <member kind="function">
      <type></type>
      <name>KalmanFilter</name>
      <anchorfile>structAdvancedFilters_1_1KalmanFilter.html</anchorfile>
      <anchor>a98b9e75660b791017f4e79266c98d877</anchor>
      <arglist>(float process_noise_param, float measurement_noise_param)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>update</name>
      <anchorfile>structAdvancedFilters_1_1KalmanFilter.html</anchorfile>
      <anchor>a821d6a47ff501dfecc2b8d944427538c</anchor>
      <arglist>(float measurement)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reset</name>
      <anchorfile>structAdvancedFilters_1_1KalmanFilter.html</anchorfile>
      <anchor>a99ec0bc17ee05c8b2e9f68a94bc7a2d1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>x</name>
      <anchorfile>structAdvancedFilters_1_1KalmanFilter.html</anchorfile>
      <anchor>abe7db25daaf07089dcee17f2c689f3e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>P</name>
      <anchorfile>structAdvancedFilters_1_1KalmanFilter.html</anchorfile>
      <anchor>a398d1dffc65492e0df8f1651fafe0421</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>Q</name>
      <anchorfile>structAdvancedFilters_1_1KalmanFilter.html</anchorfile>
      <anchor>a337131b93567489cf8173788b04974f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>R</name>
      <anchorfile>structAdvancedFilters_1_1KalmanFilter.html</anchorfile>
      <anchor>a427e5b580150a677b5765f4fa0c78be4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>initialized</name>
      <anchorfile>structAdvancedFilters_1_1KalmanFilter.html</anchorfile>
      <anchor>a9cc6cdd07d2c3730cde31428524f7783</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ModbusSensorAdapter</name>
    <filename>classModbusSensorAdapter.html</filename>
    <base>BasicSensorAdapter</base>
    <member kind="function">
      <type></type>
      <name>ModbusSensorAdapter</name>
      <anchorfile>classModbusSensorAdapter.html</anchorfile>
      <anchor>ad37d2e543823d07d8affa45e49384084</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>NPKCalibration</name>
    <filename>structNPKCalibration.html</filename>
    <member kind="variable">
      <type>float</type>
      <name>offset_nitrogen</name>
      <anchorfile>structNPKCalibration.html</anchorfile>
      <anchor>a8cc8980b9b8a4f15afd97d0000f885d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>offset_phosphorus</name>
      <anchorfile>structNPKCalibration.html</anchorfile>
      <anchor>a38399b64286f16e0d2caf20e60d65f8f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>offset_potassium</name>
      <anchorfile>structNPKCalibration.html</anchorfile>
      <anchor>ad334e95fe920bb1ff72394297dd5afe6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>is_valid</name>
      <anchorfile>structNPKCalibration.html</anchorfile>
      <anchor>a1c8434baf6e444b2759e7f63d9936b67</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>NPKCoefficients</name>
    <filename>structNPKCoefficients.html</filename>
    <member kind="function">
      <type></type>
      <name>NPKCoefficients</name>
      <anchorfile>structNPKCoefficients.html</anchorfile>
      <anchor>a5dcd60adb0b593e390e86ba44dc98833</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>NPKCoefficients</name>
      <anchorfile>structNPKCoefficients.html</anchorfile>
      <anchor>a047626c5f98844b50a9d0448b4683bf6</anchor>
      <arglist>(float dN, float dP, float dK, float eN, float eP, float eK)</arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>delta_N</name>
      <anchorfile>structNPKCoefficients.html</anchorfile>
      <anchor>ae5d8ce5696307ac0196f7569f47928fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>delta_P</name>
      <anchorfile>structNPKCoefficients.html</anchorfile>
      <anchor>a2de2e6a860f504b8dc562b7fe7a2aade</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>delta_K</name>
      <anchorfile>structNPKCoefficients.html</anchorfile>
      <anchor>aad693d5684558062897a5831121ff657</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>epsilon_N</name>
      <anchorfile>structNPKCoefficients.html</anchorfile>
      <anchor>a85c3ad4b8147742ad09765c0f0ee1b6c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>epsilon_P</name>
      <anchorfile>structNPKCoefficients.html</anchorfile>
      <anchor>a6cd47dccb1aa0db1950424361bb7696e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>epsilon_K</name>
      <anchorfile>structNPKCoefficients.html</anchorfile>
      <anchor>a1dc09a27fbd704e390407ecd43aff385</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>NPKReferences</name>
    <filename>structNPKReferences.html</filename>
    <member kind="function">
      <type></type>
      <name>NPKReferences</name>
      <anchorfile>structNPKReferences.html</anchorfile>
      <anchor>a104dd1a5f542a3e1e30811bae1bd6e03</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>NPKReferences</name>
      <anchorfile>structNPKReferences.html</anchorfile>
      <anchor>a19f04f692fa6c39ffd35247c860426e1</anchor>
      <arglist>(float n, float p, float k)</arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>nitrogen</name>
      <anchorfile>structNPKReferences.html</anchorfile>
      <anchor>a5cce1c0398a0a9e372545ea93cd88e0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>phosphorus</name>
      <anchorfile>structNPKReferences.html</anchorfile>
      <anchor>a61b1f2dd836e1a4516238d75d845a45b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>potassium</name>
      <anchorfile>structNPKReferences.html</anchorfile>
      <anchor>a47588fe7f7ef8bc14d005b7a934144aa</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>PHCalibration</name>
    <filename>structPHCalibration.html</filename>
    <member kind="variable">
      <type>std::vector&lt; CalibrationPoint &gt;</type>
      <name>points</name>
      <anchorfile>structPHCalibration.html</anchorfile>
      <anchor>affd7f0dffe3d7682185c06da8f1b0f59</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>coefficient_a</name>
      <anchorfile>structPHCalibration.html</anchorfile>
      <anchor>a7e55f6527ed360de05e0c4d79c45d819</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>coefficient_b</name>
      <anchorfile>structPHCalibration.html</anchorfile>
      <anchor>aa4cfdfe9dfcd4f53690639c562217ee8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>r_squared</name>
      <anchorfile>structPHCalibration.html</anchorfile>
      <anchor>a5b62a4a848d893a64e9444bdbfde1070</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>is_valid</name>
      <anchorfile>structPHCalibration.html</anchorfile>
      <anchor>a9d7d3135dd006b214c88fb76ac9c8cf1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RecommendationParams</name>
    <filename>structRecommendationParams.html</filename>
    <class kind="struct">RecommendationParams::Builder</class>
    <member kind="function" static="yes">
      <type>static RecommendationParams</type>
      <name>fromValues</name>
      <anchorfile>structRecommendationParams.html</anchorfile>
      <anchor>a5542073653688468ad2e0536f9cdfac8</anchor>
      <arglist>(const SensorData &amp;data, const String &amp;cropType, const String &amp;growingType, const String &amp;season, const String &amp;soilType)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Builder</type>
      <name>builder</name>
      <anchorfile>structRecommendationParams.html</anchorfile>
      <anchor>a31b3c3fc98d73f3bfc1e4f01676da5eb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>const SensorData &amp;</type>
      <name>data</name>
      <anchorfile>structRecommendationParams.html</anchorfile>
      <anchor>af5b8a9afb67c43e2f994e50a6e7f2ee6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const String &amp;</type>
      <name>cropType</name>
      <anchorfile>structRecommendationParams.html</anchorfile>
      <anchor>afaaab0627cd9504ab75639c64c944fc1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const String &amp;</type>
      <name>growingType</name>
      <anchorfile>structRecommendationParams.html</anchorfile>
      <anchor>a9733146aeb0c81901591b534d95769f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const String &amp;</type>
      <name>season</name>
      <anchorfile>structRecommendationParams.html</anchorfile>
      <anchor>a30dfee31b87ded314adeb7b29ad95962</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const String &amp;</type>
      <name>soilType</name>
      <anchorfile>structRecommendationParams.html</anchorfile>
      <anchor>ada4d4f93f369b50ef89fa8fed30452f3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RecommendationResult</name>
    <filename>structRecommendationResult.html</filename>
    <member kind="variable">
      <type>String</type>
      <name>cropType</name>
      <anchorfile>structRecommendationResult.html</anchorfile>
      <anchor>ac029fac45a2beb6f99011d7a49c919cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>growingType</name>
      <anchorfile>structRecommendationResult.html</anchorfile>
      <anchor>afdc7c64a7d23b9b24361ecce50dfda3b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>season</name>
      <anchorfile>structRecommendationResult.html</anchorfile>
      <anchor>a5ec536587029e3d69b125600f5701a2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>soilType</name>
      <anchorfile>structRecommendationResult.html</anchorfile>
      <anchor>ac4d2c62d783e2dd0904ddb89bf98c252</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>recommendations</name>
      <anchorfile>structRecommendationResult.html</anchorfile>
      <anchor>afb68ecc0cf88cb9eee2c16b3f0fe9ae6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>healthStatus</name>
      <anchorfile>structRecommendationResult.html</anchorfile>
      <anchor>a0f34697f68ccd584285b4142a97bc4f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>scientificNotes</name>
      <anchorfile>structRecommendationResult.html</anchorfile>
      <anchor>a9111704f809f1092a8b643509117e1ba</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RecValues</name>
    <filename>structRecValues.html</filename>
    <member kind="variable">
      <type>float</type>
      <name>t</name>
      <anchorfile>structRecValues.html</anchorfile>
      <anchor>ad5e68754a138f0b3d4a2fcc8a6d41846</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>hum</name>
      <anchorfile>structRecValues.html</anchorfile>
      <anchor>af5f8d5f555bee5aa71f30afabce004fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>ec</name>
      <anchorfile>structRecValues.html</anchorfile>
      <anchor>a93613bd3e62e62e2e5348dbcdbf7141c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>ph</name>
      <anchorfile>structRecValues.html</anchorfile>
      <anchor>ac357ff48495f333eaf3e2b92fd21cf43</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>n</name>
      <anchorfile>structRecValues.html</anchorfile>
      <anchor>a33f921599e1cd50cdd79c3a323ec726f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>p</name>
      <anchorfile>structRecValues.html</anchorfile>
      <anchor>a16338a277db1e53fc9f557a30777d3c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>k</name>
      <anchorfile>structRecValues.html</anchorfile>
      <anchor>a52e88e1a415453e74e04ab301231bdb6</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>SensorCache</name>
    <filename>structSensorCache.html</filename>
    <member kind="variable">
      <type>SensorData</type>
      <name>data</name>
      <anchorfile>structSensorCache.html</anchorfile>
      <anchor>a6bd2db19de6e4e64d0c3dbef2a220dd6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>is_valid</name>
      <anchorfile>structSensorCache.html</anchorfile>
      <anchor>add741c7682063714899826a20fe363f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned long</type>
      <name>timestamp</name>
      <anchorfile>structSensorCache.html</anchorfile>
      <anchor>ab5ba28cf93b91b1560a5d26926b74a61</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>SensorCalibration</name>
    <filename>structSensorCalibration.html</filename>
    <member kind="variable">
      <type>String</type>
      <name>sensor_id</name>
      <anchorfile>structSensorCalibration.html</anchorfile>
      <anchor>a7df4dce3d4ec988eb91a5b2130c35bf4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>calibration_date</name>
      <anchorfile>structSensorCalibration.html</anchorfile>
      <anchor>a5f97c4c9e3386caef11f1f1d24031f22</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>PHCalibration</type>
      <name>ph</name>
      <anchorfile>structSensorCalibration.html</anchorfile>
      <anchor>ad2978d2635dafcaf7e84aabbc63b9ddb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ECCalibration</type>
      <name>ec</name>
      <anchorfile>structSensorCalibration.html</anchorfile>
      <anchor>af6b09d60bbadce1c33b7fc2cb95a0ac9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>NPKCalibration</type>
      <name>npk</name>
      <anchorfile>structSensorCalibration.html</anchorfile>
      <anchor>ab5ccbc59d2b79f8ee7615ae89e1994cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>is_complete</name>
      <anchorfile>structSensorCalibration.html</anchorfile>
      <anchor>a8415551d0a8d36fd2ce35c70f2eb98b1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SensorCalibrationService</name>
    <filename>classSensorCalibrationService.html</filename>
    <base>ISensorCalibrationService</base>
    <member kind="function">
      <type></type>
      <name>SensorCalibrationService</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a443e4ac913b89829d484bcc6580bf916</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~SensorCalibrationService</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a855758c8defb35aade62663de3fb23fa</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>applyCalibration</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a40a2ed991a3ebdf300869c8f59312b9d</anchor>
      <arglist>(SensorData &amp;data, SoilProfile profile) override</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>applySingleCalibration</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>ac073021d9df7c94b7407eaaf54dbfd7b</anchor>
      <arglist>(float rawValue, SoilProfile profile) override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>validateCalibrationData</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>addec9f150b9de4e7716e12e457bb916b</anchor>
      <arglist>(const SensorData &amp;data) override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>loadCalibrationTable</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>aa600ed09463d2bd237d56a03f999498f</anchor>
      <arglist>(const String &amp;csvData, SoilProfile profile) override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>hasCalibrationTable</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>abb57988a85d77e1ff27cdd1ea8039e3e</anchor>
      <arglist>(SoilProfile profile) const override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clearCalibrationTable</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>ad6dd5beb925f8ed0a643dc90e58e5dba</anchor>
      <arglist>(SoilProfile profile) override</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>getCalibrationStatus</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a829a61a49e6edf9bd55d2285c1c7581a</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isCalibrationComplete</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a1eab743addca249d06ed5bff08339d38</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>addPHCalibrationPoint</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a0876648a825a42d6e13fe12f1da3d027</anchor>
      <arglist>(float expected, float measured)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>addECCalibrationPoint</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a9837694143edfe2123188f790b1dd84e</anchor>
      <arglist>(float expected, float measured)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>setNPKCalibrationPoint</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a6a2bf82cd1ce14f8cff1404959772479</anchor>
      <arglist>(float n, float p, float k)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>calculatePHCalibration</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a42089b7ee8916f3cb322f1815da47a16</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>calculateECCalibration</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a7c6f752577b999d4ede62a7d7e90ec29</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>exportCalibrationToJSON</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>aff7e797251e50a5b32f51aca2819e529</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>importCalibrationFromJSON</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a76bddb9c474728779243828ac736757b</anchor>
      <arglist>(const String &amp;jsonData)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resetCalibration</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a369106a8349078064a3e980340f42de3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SensorCalibrationService</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a443e4ac913b89829d484bcc6580bf916</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>addPHCalibrationPoint</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a0876648a825a42d6e13fe12f1da3d027</anchor>
      <arglist>(float expected, float measured)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>addECCalibrationPoint</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a9837694143edfe2123188f790b1dd84e</anchor>
      <arglist>(float expected, float measured)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>setNPKCalibrationPoint</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a391efc96fcf9ec612761a269be44385d</anchor>
      <arglist>(float measured_n, float measured_p, float measured_k)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>calculatePHCalibration</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a42089b7ee8916f3cb322f1815da47a16</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>calculateECCalibration</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a7c6f752577b999d4ede62a7d7e90ec29</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>calculateNPKCalibration</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>ac32dabe371de4d9ec9eb59f3d37aed20</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>applyCalibration</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a925828aa33345f9fd5bf8cee89d43003</anchor>
      <arglist>(float &amp;ph, float &amp;ec, float &amp;nitrogen, float &amp;phosphorus, float &amp;potassium)</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>exportCalibrationToJSON</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>aff7e797251e50a5b32f51aca2819e529</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>importCalibrationFromJSON</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a1f6fc3abea9eff4975604f2c426db7ed</anchor>
      <arglist>(const String &amp;json_data)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isCalibrationComplete</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a1eab743addca249d06ed5bff08339d38</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>String</type>
      <name>getCalibrationStatus</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a829a61a49e6edf9bd55d2285c1c7581a</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resetCalibration</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a369106a8349078064a3e980340f42de3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>validateCalibration</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a00656cab3331194e8f8411c2f57a1c88</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static size_t</type>
      <name>getCalibrationPointsCount</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>a12aed6aa1eedb14700981d57ceacece4</anchor>
      <arglist>(SoilProfile profile, const String &amp;sensorType)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static String</type>
      <name>exportCalibrationTable</name>
      <anchorfile>classSensorCalibrationService.html</anchorfile>
      <anchor>ac663f33782030bade32a75a0f5766de5</anchor>
      <arglist>(SoilProfile profile)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SensorCompensationService</name>
    <filename>classSensorCompensationService.html</filename>
    <base>ISensorCompensationService</base>
    <member kind="function">
      <type></type>
      <name>SensorCompensationService</name>
      <anchorfile>classSensorCompensationService.html</anchorfile>
      <anchor>a433592cef86adadec3a1d137a3ad7821</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~SensorCompensationService</name>
      <anchorfile>classSensorCompensationService.html</anchorfile>
      <anchor>a82260d6f41e363d42bb69b6648bc7aa3</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>applyCompensation</name>
      <anchorfile>classSensorCompensationService.html</anchorfile>
      <anchor>a6b4ab9b9b0c290dc067607ec63bd0b78</anchor>
      <arglist>(SensorData &amp;data, SoilType soilType) override</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>correctEC</name>
      <anchorfile>classSensorCompensationService.html</anchorfile>
      <anchor>ac3c00a1819376ba24e83b7ff38974bcf</anchor>
      <arglist>(float ec25, SoilType soilType, float temperature, float humidity) override</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>correctPH</name>
      <anchorfile>classSensorCompensationService.html</anchorfile>
      <anchor>adf4ffe824326e596f096d48a8d8690e5</anchor>
      <arglist>(float temperature, float phRaw) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>correctNPK</name>
      <anchorfile>classSensorCompensationService.html</anchorfile>
      <anchor>aff11931834f075268d91639ac307da14</anchor>
      <arglist>(float temperature, float humidity, SoilType soilType, NPKReferences &amp;npk) override</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>getArchieCoefficient</name>
      <anchorfile>classSensorCompensationService.html</anchorfile>
      <anchor>a08e4ed05ff96d3bb573a22372d9db076</anchor>
      <arglist>(SoilType soilType) const override</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>getPorosity</name>
      <anchorfile>classSensorCompensationService.html</anchorfile>
      <anchor>ac555cd253cbdbbf14ab13eb0da8544a5</anchor>
      <arglist>(SoilType soilType) const override</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>validateCompensationInputs</name>
      <anchorfile>classSensorCompensationService.html</anchorfile>
      <anchor>a485ee63afccf5439d7b094726522ec19</anchor>
      <arglist>(SoilType soilType, float humidity, float temperature) const override</arglist>
    </member>
    <member kind="function">
      <type>SoilParameters</type>
      <name>getSoilParameters</name>
      <anchorfile>classSensorCompensationService.html</anchorfile>
      <anchor>a7aadee4bf1e0b43eae1e22e9d4384676</anchor>
      <arglist>(SoilType soilType) const</arglist>
    </member>
    <member kind="function">
      <type>ArchieCoefficients</type>
      <name>getArchieCoefficients</name>
      <anchorfile>classSensorCompensationService.html</anchorfile>
      <anchor>aeaa7aaae9b5877afa6abb189ca0f1ed6</anchor>
      <arglist>(SoilType soilType) const</arglist>
    </member>
    <member kind="function">
      <type>NPKCoefficients</type>
      <name>getNPKCoefficients</name>
      <anchorfile>classSensorCompensationService.html</anchorfile>
      <anchor>ae7853ed28165e565a6e3d65bbd548d30</anchor>
      <arglist>(SoilType soilType) const</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>SensorData</name>
    <filename>structSensorData.html</filename>
    <member kind="variable">
      <type>float</type>
      <name>temperature</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a0a1593ad64f79756c14d208ce7b17adb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>humidity</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a29ce8f1d86589b1c5af915ec961742f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>ec</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>aa990acd0e7ae2bdc99a54069ffe36e41</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>ph</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>ae71019c6d425e1d48eca40c9ef0157f9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>nitrogen</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a129ffaa95987191cf7c405525e156b4d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>phosphorus</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a05e9b50851e82dba2487bf1b0c770539</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>potassium</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>afa3d7a9345781758cec2e8bac97ac1d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>moisture</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a3e1daf37ef4f33ec73aa3bd93fc3d42f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>conductivity</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>aba644ee83b3bceceea0b6325fe00257e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>firmware_version</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>afde3c50727e3e15ce3850afa8d73eff8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>error_status</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>ac4ca11b4574092a5d653f9c3a838e218</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>valid</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a232e987cbc8cfe2b9977416c9e25554f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>isValid</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a6985f18dc44b23c48323f1f378e594db</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned long</type>
      <name>last_update</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>ad603dc902dc414959a862b8e4b2777f9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned long</type>
      <name>timestamp</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>ac3cf8281a41a979e6d5d894b45c05ca0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>prev_temperature</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>afb0e9f9ada5f63dbbe113a85b4ce2299</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>prev_humidity</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>aef31dc7b08499063bb728d0585cf4438</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>prev_ec</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>aa8a37e0d8651c3d73412c40f42934c2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>prev_ph</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a895c584f0848993edcceceba4bd66456</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>prev_nitrogen</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>aa45648f478825f9cb4e327f3bba45944</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>prev_phosphorus</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a2b6a3e91720650c6d5076b47a1548b94</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>prev_potassium</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>abb4cf863801722def12904f626746a38</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned long</type>
      <name>last_mqtt_publish</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>ae86305145f71a026a5c398edab06f978</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>temp_buffer</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>aaf53e2b2f0d5c70294c365f0d3990a9b</anchor>
      <arglist>[15]</arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>hum_buffer</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a2202526bdd825aac6317285d8f6d679f</anchor>
      <arglist>[15]</arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>ec_buffer</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a43150919a4c14d363c38a617bb1ff12b</anchor>
      <arglist>[15]</arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>ph_buffer</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a69d50c2fa4a0055f90047fc19b2a7930</anchor>
      <arglist>[15]</arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>n_buffer</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a361f886f983491129d039fc97372fdcb</anchor>
      <arglist>[15]</arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>p_buffer</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a7979bb38cf97b270df57bf7621259a0d</anchor>
      <arglist>[15]</arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>k_buffer</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a47cd6d9e8ac34c2eaaf7517790d1f318</anchor>
      <arglist>[15]</arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>buffer_index</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a0b41fc80d82705a0a7adc521795623d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>buffer_filled</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>acf88a59ba1b4c48bcbe74e853e3ab7e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>raw_temperature</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>ad4f2c5ac6721efbfb394d394e74cc369</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>raw_humidity</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a03cdd5ca84b4a315fc15b2053a976c44</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>raw_ec</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>ac3f0a2a0fdf1fc1f1ccd9da06bf1aaff</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>raw_ph</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a52fa7ffba8c0e5a3177f77e3b2583b20</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>raw_nitrogen</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a6c18579c1d8089ae77459f3ff81a3c33</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>raw_phosphorus</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>ab84101974d645fbb4e64be1ac4f57ccb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>raw_potassium</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a6918481c09e8ca124cce3d630ccc1f4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>recentIrrigation</name>
      <anchorfile>structSensorData.html</anchorfile>
      <anchor>a209057037cdfa0f908f128c11a581f4a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>SensorValidationResult</name>
    <filename>structSensorValidationResult.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>isValid</name>
      <anchorfile>structSensorValidationResult.html</anchorfile>
      <anchor>acfa046237a82d060e944d05e093d26bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; ValidationError &gt;</type>
      <name>errors</name>
      <anchorfile>structSensorValidationResult.html</anchorfile>
      <anchor>a41444c647d77a9f5f7b4c7d38e2abb8a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>SoilParameters</name>
    <filename>structSoilParameters.html</filename>
    <member kind="function">
      <type></type>
      <name>SoilParameters</name>
      <anchorfile>structSoilParameters.html</anchorfile>
      <anchor>a82a2f5722a3c73edb72eab37070c05eb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SoilParameters</name>
      <anchorfile>structSoilParameters.html</anchorfile>
      <anchor>a60f1d32e3ae4dfefbc093ac0ca952a7b</anchor>
      <arglist>(float por, float density, float capacity)</arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>porosity</name>
      <anchorfile>structSoilParameters.html</anchorfile>
      <anchor>a91f6c1b2df01751d918c701c31422c6e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>bulkDensity</name>
      <anchorfile>structSoilParameters.html</anchorfile>
      <anchor>a7aac06cefc41d9d25bbab65555695666</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>fieldCapacity</name>
      <anchorfile>structSoilParameters.html</anchorfile>
      <anchor>a8127688bbf9c43a6f2bdc9f8241ff1f8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>AdvancedFilters::StatisticsBuffer</name>
    <filename>structAdvancedFilters_1_1StatisticsBuffer.html</filename>
    <member kind="function">
      <type></type>
      <name>StatisticsBuffer</name>
      <anchorfile>structAdvancedFilters_1_1StatisticsBuffer.html</anchorfile>
      <anchor>aa288cb4f070eb6a41463ea41e3e9b21b</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="variable">
      <type>std::array&lt; float, STATISTICS_WINDOW_SIZE &gt;</type>
      <name>values</name>
      <anchorfile>structAdvancedFilters_1_1StatisticsBuffer.html</anchorfile>
      <anchor>a38ba9dd659e8d14fc496f79b3f6a7916</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>index</name>
      <anchorfile>structAdvancedFilters_1_1StatisticsBuffer.html</anchorfile>
      <anchor>a53343f27efdaefc427f53839917dc169</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>filled</name>
      <anchorfile>structAdvancedFilters_1_1StatisticsBuffer.html</anchorfile>
      <anchor>afac932f69242b718818af94cf6d009dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>mean</name>
      <anchorfile>structAdvancedFilters_1_1StatisticsBuffer.html</anchorfile>
      <anchor>a11e0869baed079b0d05e19b58b50311e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>std_dev</name>
      <anchorfile>structAdvancedFilters_1_1StatisticsBuffer.html</anchorfile>
      <anchor>a4be18d270f7e234564584c6e9c57c2dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>valid</name>
      <anchorfile>structAdvancedFilters_1_1StatisticsBuffer.html</anchorfile>
      <anchor>a7fb29ed340103950ae3ae82222e788cb</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>TechnicalDebtMetrics</name>
    <filename>structTechnicalDebtMetrics.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>code_smells</name>
      <anchorfile>structTechnicalDebtMetrics.html</anchorfile>
      <anchor>a4773c49c02364f6bb44170689e5289bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>duplicated_lines</name>
      <anchorfile>structTechnicalDebtMetrics.html</anchorfile>
      <anchor>ad65a2aec2bf57710c6ce49d3e5f37aad</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>complexity_issues</name>
      <anchorfile>structTechnicalDebtMetrics.html</anchorfile>
      <anchor>a33069ab52bf8c409528b7f910b98213b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>security_hotspots</name>
      <anchorfile>structTechnicalDebtMetrics.html</anchorfile>
      <anchor>a1943cf949cadf24adf12d9b4d7826447</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>maintainability_rating</name>
      <anchorfile>structTechnicalDebtMetrics.html</anchorfile>
      <anchor>a91e45a409e2a8bf74a1e2a14e66ee9b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>debt_ratio</name>
      <anchorfile>structTechnicalDebtMetrics.html</anchorfile>
      <anchor>ad803b6eba16938f3dc80abcc0ae23b87</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>coverage</name>
      <anchorfile>structTechnicalDebtMetrics.html</anchorfile>
      <anchor>a86add9e411728c81304ac1363524e6f9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>TestSummary</name>
    <filename>structTestSummary.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>total</name>
      <anchorfile>structTestSummary.html</anchorfile>
      <anchor>a47920eeb88e47e75df7720dc15e8b607</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>passed</name>
      <anchorfile>structTestSummary.html</anchorfile>
      <anchor>acdeb57b76f73912124b9dc10e010c47f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>failed</name>
      <anchorfile>structTestSummary.html</anchorfile>
      <anchor>a083f34fa2b73ba5a06e7000c28927b96</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>float</type>
      <name>success_rate</name>
      <anchorfile>structTestSummary.html</anchorfile>
      <anchor>aeebadaa0f9fe3b629218b308729db8f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>timestamp</name>
      <anchorfile>structTestSummary.html</anchorfile>
      <anchor>a0bf3727bdff9eafa88b9a02fc9927026</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ValidationError</name>
    <filename>structValidationError.html</filename>
    <member kind="variable">
      <type>String</type>
      <name>field</name>
      <anchorfile>structValidationError.html</anchorfile>
      <anchor>afe37331a5cad29591a36872c97118ebe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>message</name>
      <anchorfile>structValidationError.html</anchorfile>
      <anchor>a7d8199508641c0ffe90e1b15fe3cc82c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ValidationRange</name>
    <filename>structValidationRange.html</filename>
    <class kind="struct">ValidationRange::Builder</class>
    <member kind="function" static="yes">
      <type>static Builder</type>
      <name>builder</name>
      <anchorfile>structValidationRange.html</anchorfile>
      <anchor>a465d11c47b8ac7b80ac1061471580d12</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>minValue</name>
      <anchorfile>structValidationRange.html</anchorfile>
      <anchor>af7ee76e1f5187f5776035b2e630a9e35</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>maxValue</name>
      <anchorfile>structValidationRange.html</anchorfile>
      <anchor>a97271bcc2cdb5d5d5d8f80bf94b4288e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ValidationResult</name>
    <filename>structValidationResult.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>isValid</name>
      <anchorfile>structValidationResult.html</anchorfile>
      <anchor>ae2c6c08be51aa32f2b8991bbcc9c4625</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>String</type>
      <name>message</name>
      <anchorfile>structValidationResult.html</anchorfile>
      <anchor>a53fbac7dfa578d7c9532106dbf325eba</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>AdvancedFilters</name>
    <filename>namespaceAdvancedFilters.html</filename>
    <class kind="struct">AdvancedFilters::ECFilterState</class>
    <class kind="struct">AdvancedFilters::ExponentialSmoothingState</class>
    <class kind="struct">AdvancedFilters::KalmanFilter</class>
    <class kind="struct">AdvancedFilters::StatisticsBuffer</class>
    <member kind="enumeration">
      <type></type>
      <name>FilterType</name>
      <anchorfile>namespaceAdvancedFilters.html</anchorfile>
      <anchor>a2ca4878176ecbc18c74ae65f1d54cd2c</anchor>
      <arglist></arglist>
      <enumvalue file="namespaceAdvancedFilters.html" anchor="a2ca4878176ecbc18c74ae65f1d54cd2cab976538812ef6e3e5881e245d5fc3c76">TEMPERATURE</enumvalue>
      <enumvalue file="namespaceAdvancedFilters.html" anchor="a2ca4878176ecbc18c74ae65f1d54cd2ca1bfd5cb0758595fd2f392b93a92e8fff">HUMIDITY</enumvalue>
      <enumvalue file="namespaceAdvancedFilters.html" anchor="a2ca4878176ecbc18c74ae65f1d54cd2ca3fd6b696867d70225deda7868308679b">EC</enumvalue>
      <enumvalue file="namespaceAdvancedFilters.html" anchor="a2ca4878176ecbc18c74ae65f1d54cd2caa25496ebf095e4198da4088449c83ac6">PH</enumvalue>
      <enumvalue file="namespaceAdvancedFilters.html" anchor="a2ca4878176ecbc18c74ae65f1d54cd2ca89b53d9262cdc7a36ed5f2986aaefdd8">NITROGEN</enumvalue>
      <enumvalue file="namespaceAdvancedFilters.html" anchor="a2ca4878176ecbc18c74ae65f1d54cd2ca5758449024f3eb7bc460841d9b04749b">PHOSPHORUS</enumvalue>
      <enumvalue file="namespaceAdvancedFilters.html" anchor="a2ca4878176ecbc18c74ae65f1d54cd2ca5ffc0bd406ecdc2349ea5d8f8f47c6bf">POTASSIUM</enumvalue>
    </member>
    <member kind="function">
      <type>void</type>
      <name>applyAdvancedFiltering</name>
      <anchorfile>namespaceAdvancedFilters.html</anchorfile>
      <anchor>abfbf0e02e94828a4ad01afbfcc50bbca</anchor>
      <arglist>(SensorData &amp;data)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resetAllFilters</name>
      <anchorfile>namespaceAdvancedFilters.html</anchorfile>
      <anchor>a281a34e902e538bea51e34fcceff5c76</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>logFilterStatistics</name>
      <anchorfile>namespaceAdvancedFilters.html</anchorfile>
      <anchor>aa16fa718fe4b71fc233facdce4cbaca9</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>CalibrationManager</name>
    <filename>namespaceCalibrationManager.html</filename>
    <member kind="function">
      <type>const char *</type>
      <name>profileToFilename</name>
      <anchorfile>namespaceCalibrationManager.html</anchorfile>
      <anchor>a24c0b11ea08e757ae404289fa38e33b3</anchor>
      <arglist>(SoilProfile)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>init</name>
      <anchorfile>namespaceCalibrationManager.html</anchorfile>
      <anchor>aee9fe56519e509e44d7f936a7a4c3613</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>saveCsv</name>
      <anchorfile>namespaceCalibrationManager.html</anchorfile>
      <anchor>ade735ff876ef578fafeb7c46676bcf82</anchor>
      <arglist>(SoilProfile profile, Stream &amp;fileStream)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>loadTable</name>
      <anchorfile>namespaceCalibrationManager.html</anchorfile>
      <anchor>a15db87f3fcb09c7f4c5abc69009cef89</anchor>
      <arglist>(SoilProfile profile, CalibrationEntry *outBuffer, size_t maxEntries, size_t &amp;outCount)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>hasTable</name>
      <anchorfile>namespaceCalibrationManager.html</anchorfile>
      <anchor>a1ee0f1f244cb68eaa7e47a79deb452b6</anchor>
      <arglist>(SoilProfile profile)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>deleteTable</name>
      <anchorfile>namespaceCalibrationManager.html</anchorfile>
      <anchor>a14e21e7fb1aa46aa781c20ee07271d12</anchor>
      <arglist>(SoilProfile profile)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>applyCalibration</name>
      <anchorfile>namespaceCalibrationManager.html</anchorfile>
      <anchor>abb872938a17efb733eaf531bb5c0a1dc</anchor>
      <arglist>(float rawValue, SoilProfile profile)</arglist>
    </member>
  </compound>
</tagfile>
