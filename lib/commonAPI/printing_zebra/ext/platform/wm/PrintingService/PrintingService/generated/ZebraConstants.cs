using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;

namespace rho
{
    namespace PrinterZebraImpl
    {
        public class ZebraConstants
        {
            public const string HK_ADDITIONAL_STRING_OPTION = "additionalStringOption";

            public const string HK_CONNECTION_TYPE = "connectionType";

            public const string HK_DEVICE_ADDRESS = "deviceAddress";

            public const string HK_DEVICE_PORT = "devicePort";

            public const string HK_FILE_NAMES = "fileNames";

            public const string HK_HEIGHT = "height";

            public const string HK_IS_INSIDE_FORMAT = "isInsideFormat";

            public const string HK_MESSAGE = "message";

            public const string HK_PRINTER_ID = "printerID";

            public const string HK_PRINTER_TYPE = "printerType";

            public const string HK_PROPERTY_WITH_INT_VALUE = "propertyWithIntValue";

            public const string HK_PRPERTY_WITH_STRING_VALUE = "prpertyWithStringValue";

            public const string HK_STATUS = "status";

            public const string HK_TIMEOUT = "timeout";

            public const string HK_WIDTH = "width";

            public const string PROPERTY_CONNECTION_TYPE = "connectionType";

            public const string PROPERTY_CONTROL_LANGUAGE = "controlLanguage";

            public const string PROPERTY_DEVICE_ADDRESS = "deviceAddress";

            public const string PROPERTY_DEVICE_NAME = "deviceName";

            public const string PROPERTY_DEVICE_PORT = "devicePort";

            public const string PROPERTY_ID = "ID";

            public const string PROPERTY_IS_CONNECTED = "isConnected";

            public const string PROPERTY_MAX_TIMEOUT_FOR_OPEN = "maxTimeoutForOpen";

            public const string PROPERTY_MAX_TIMEOUT_FOR_READ = "maxTimeoutForRead";

            public const string PROPERTY_PRINTER_EVENT_CALLBACK = "printerEventCallback";

            public const string PROPERTY_PRINTER_TYPE = "printerType";

            public const string PROPERTY_TIME_TO_WAIT_AFTER_READ_IN_MILLISECONDS = "timeToWaitAfterReadInMilliseconds";

            public const string PROPERTY_TIME_TO_WAIT_AFTER_WRITE_IN_MILLISECONDS = "timeToWaitAfterWriteInMilliseconds";

            public const string PROPERTY_TIME_TO_WAIT_FOR_MORE_DATA = "timeToWaitForMoreData";

            public const string HK_STRING_RESPONCE = "stringResponce";

            //////////////////////////////////////////////////////////////////////////

            public const string CONNECTION_TYPE_ANY = "CONNECTION_TYPE_ANY";
            public const string CONNECTION_TYPE_BLUETOOTH = "CONNECTION_TYPE_BLUETOOTH";
            public const string CONNECTION_TYPE_ON_BOARD = "CONNECTION_TYPE_ON_BOARD";
            public const string CONNECTION_TYPE_TCP = "CONNECTION_TYPE_TCP";
            public const string CONNECTION_TYPE_USB = "CONNECTION_TYPE_USB";
            public const string DISCOVERY_MODE_DIRECTED_BROADCAST = "DISCOVERY_MODE_DIRECTED_BROADCAST";
            public const string DISCOVERY_MODE_LOCAL_BROADCAST = "DISCOVERY_MODE_LOCAL_BROADCAST";
            public const string DISCOVERY_MODE_MULTICAST = "DISCOVERY_MODE_MULTICAST";
            public const string DISCOVERY_MODE_SUBNET_SEARCH = "DISCOVERY_MODE_SUBNET_SEARCH";
            public const string ERROR_BAD_FILE_DIRECTORY_ENTRY = "ERROR_BAD_FILE_DIRECTORY_ENTRY";
            public const string ERROR_INVALID_DISCOVERY_HOP_COUNT = "ERROR_INVALID_DISCOVERY_HOP_COUNT";
            public const string ERROR_INVALID_FILE_NAME = "ERROR_INVALID_FILE_NAME";
            public const string ERROR_INVALID_FORMAT_NAME = "ERROR_INVALID_FORMAT_NAME";
            public const string ERROR_INVALID_PRINTER_DRIVE_LETTER = "ERROR_INVALID_PRINTER_DRIVE_LETTER";
            public const string ERROR_INVALID_PRINTER_LANGUAGE = "ERROR_INVALID_PRINTER_LANGUAGE";
            public const string ERROR_MALFORMED_FORMAT_FIELD_NUMBER = "ERROR_MALFORMED_FORMAT_FIELD_NUMBER";
            public const string ERROR_MALFORMED_NETWORK_DISCOVERY_ADDRESS = "ERROR_MALFORMED_NETWORK_DISCOVERY_ADDRESS";
            public const string ERROR_MALFORMED_PRINTER_STATUS_RESPONSE = "ERROR_MALFORMED_PRINTER_STATUS_RESPONSE";
            public const string ERROR_NETWORK_ERROR_DURING_DISCOVERY = "ERROR_NETWORK_ERROR_DURING_DISCOVERY";
            public const string ERROR_NO_CONNECTION = "ERROR_NO_CONNECTION";
            public const string ERROR_READ_FAILURE = "ERROR_READ_FAILURE";
            public const string ERROR_UNKNOWN_PRINTER_LANGUAGE = "ERROR_UNKNOWN_PRINTER_LANGUAGE";
            public const string ERROR_WRITE_FAILURE = "ERROR_WRITE_FAILURE";
            public const string PRINTER_LANGUAGE_CPCL = "PRINTER_LANGUAGE_CPCL";
            public const string PRINTER_LANGUAGE_EPS = "PRINTER_LANGUAGE_EPS";
            public const string PRINTER_LANGUAGE_ZPL = "PRINTER_LANGUAGE_ZPL";
            public const string PRINTER_STATE_IS_BATTERY_LOW = "PRINTER_STATE_IS_BATTERY_LOW";
            public const string PRINTER_STATE_IS_COVER_OPENED = "PRINTER_STATE_IS_COVER_OPENED";
            public const string PRINTER_STATE_IS_DRAWER_OPENED = "PRINTER_STATE_IS_DRAWER_OPENED";
            public const string PRINTER_STATE_IS_HEAD_COLD = "PRINTER_STATE_IS_HEAD_COLD";
            public const string PRINTER_STATE_IS_HEAD_OPEN = "PRINTER_STATE_IS_HEAD_OPEN";
            public const string PRINTER_STATE_IS_HEAD_TOO_HOT = "PRINTER_STATE_IS_HEAD_TOO_HOT";
            public const string PRINTER_STATE_IS_PAPER_OUT = "PRINTER_STATE_IS_PAPER_OUT";
            public const string PRINTER_STATE_IS_PARTIAL_FORMAT_IN_PROGRESS = "PRINTER_STATE_IS_PARTIAL_FORMAT_IN_PROGRESS";
            public const string PRINTER_STATE_IS_PAUSED = "PRINTER_STATE_IS_PAUSED";
            public const string PRINTER_STATE_IS_READY_TO_PRINT = "PRINTER_STATE_IS_READY_TO_PRINT";
            public const string PRINTER_STATE_IS_RECEIVE_BUFFER_FULL = "PRINTER_STATE_IS_RECEIVE_BUFFER_FULL";
            public const string PRINTER_STATE_IS_RIBBON_OUT = "PRINTER_STATE_IS_RIBBON_OUT";
            public const string PRINTER_STATE_LABELS_REMAINING_IN_BATCH = "PRINTER_STATE_LABELS_REMAINING_IN_BATCH";
            public const string PRINTER_STATE_LABEL_LENGTH_IN_DOTS = "PRINTER_STATE_LABEL_LENGTH_IN_DOTS";
            public const string PRINTER_STATE_NUMBER_OF_FORMATS_IN_RECEIVE_BUFFER = "PRINTER_STATE_NUMBER_OF_FORMATS_IN_RECEIVE_BUFFER";
            public const string PRINTER_STATE_PRINT_MODE = "PRINTER_STATE_PRINT_MODE";
            public const string PRINTER_STATUS_ERROR = "PRINTER_STATUS_ERROR";
            public const string PRINTER_STATUS_ERR_IO = "PRINTER_STATUS_ERR_IO";
            public const string PRINTER_STATUS_ERR_MEMORY = "PRINTER_STATUS_ERR_MEMORY";
            public const string PRINTER_STATUS_ERR_NETWORK = "PRINTER_STATUS_ERR_NETWORK";
            public const string PRINTER_STATUS_ERR_NOT_CONNECTED = "PRINTER_STATUS_ERR_NOT_CONNECTED";
            public const string PRINTER_STATUS_ERR_NOT_FOUND = "PRINTER_STATUS_ERR_NOT_FOUND";
            public const string PRINTER_STATUS_ERR_PARAM = "PRINTER_STATUS_ERR_PARAM";
            public const string PRINTER_STATUS_ERR_PROCESSING = "PRINTER_STATUS_ERR_PROCESSING";
            public const string PRINTER_STATUS_ERR_RESPONSE = "PRINTER_STATUS_ERR_RESPONSE";
            public const string PRINTER_STATUS_ERR_TIMEOUT = "PRINTER_STATUS_ERR_TIMEOUT";
            public const string PRINTER_STATUS_ERR_UNSUPPORTED = "PRINTER_STATUS_ERR_UNSUPPORTED";
            public const string PRINTER_STATUS_SUCCESS = "PRINTER_STATUS_SUCCESS";
            public const string PRINTER_TYPE_ANY = "PRINTER_TYPE_ANY";
            public const string PRINTER_TYPE_APD = "PRINTER_TYPE_APD";
            public const string PRINTER_TYPE_EPSON = "PRINTER_TYPE_EPSON";
            public const string PRINTER_TYPE_NATIVE = "PRINTER_TYPE_NATIVE";
            public const string PRINTER_TYPE_ZEBRA = "PRINTER_TYPE_ZEBRA";
            public const string PRINT_MODE_APPLICATOR = "PRINT_MODE_APPLICATOR";
            public const string PRINT_MODE_CUTTER = "PRINT_MODE_CUTTER";
            public const string PRINT_MODE_DELAYED_CUT = "PRINT_MODE_DELAYED_CUT";
            public const string PRINT_MODE_KIOSK = "PRINT_MODE_KIOSK";
            public const string PRINT_MODE_LINERLESS_PEEL = "PRINT_MODE_LINERLESS_PEEL";
            public const string PRINT_MODE_LINERLESS_REWIND = "PRINT_MODE_LINERLESS_REWIND";
            public const string PRINT_MODE_PARTIAL_CUTTER = "PRINT_MODE_PARTIAL_CUTTER";
            public const string PRINT_MODE_PEEL_OFF = "PRINT_MODE_PEEL_OFF";
            public const string PRINT_MODE_REWIND = "PRINT_MODE_REWIND";
            public const string PRINT_MODE_RFID = "PRINT_MODE_RFID";
            public const string PRINT_MODE_TEAR_OFF = "PRINT_MODE_TEAR_OFF";
            public const string PRINT_MODE_UNKNOWN = "PRINT_MODE_UNKNOWN";

            public const Int32 readyToPrintTimeout = 4000;
            public const Int32 connectionTimeout = 3000;
        }
    }
}
