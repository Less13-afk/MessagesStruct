#include "MessagesStruct.h"
using namespace std;

enum class MessagesId : unsigned char
{ //Перечисление ID сообщений
	SysInfo = 1, // ID сообщения MessageHeartBeat
	GyroInfo, // ID сообщения MessageGyroInfo
	AccelInfo,// ID сообщения MessageAccelInfo
	GpsInfo, // ID сообщения MessageGpsInfo
	InertialInfo,// ID сообщения MessageInertialInfo
	BatteryInfo,// ID сообщения MessageBatteryInfo
	Ack,// ID сообщения MessageCommandACK
	StatusCommand, // ID сообщения со статусом команды
	StatusAllCommands, // ID сообщения со статусом всех команд
	Command // ID сообщения с командой
};

enum class CommandId : unsigned char
{
	ChangeNav = 1,// ID Команды CommandChangeNav
	ChangeSpeed,// ID Команды CommandChangeSpeed
	Land,// ID Команды Command (поскадка)
	GoHome,// ID Команды Command (возврат домой)
	StopEngine,// ID Команды Command (остановка двигателей)
	StartEngine,// ID Команды Command (запуск двигателей)
	Pause,// ID Команды Command (пауза и удержание текущей позиции)
	Continue,// ID Команды Command (продолжение полета к точке после паузы)
	GoTo,// ID Команды CommandGoTo
	RequestMessage, // ID команды запроса сообщения
	RequestStatusCommand // ID команды запроса статуса команды по ее ID
};

enum class ErrorCode : unsigned short
{
	// Заполнить структуру возможных ошибок
};

#pragma pack (push, 1)
struct Header //Header сообщений
{
	unsigned char stx;
	MessagesId msgId;
	unsigned char lenPayload;
	unsigned short crc;
	unsigned char payload[];
};
struct PayloadMessageBatteryInfo // Сообщение с данными о состояни АКБ (частота публикации сообщения – 4 Гц)
{
	unsigned char perCharge; //Процент заряда АКБ
	float voltage; // Текущее напряжение АКБ
	float amperage; // Текущий ток АКБ
	float temp; // Температура АКБ
	float totalPower; // общая потребляемая ПЛК, датчиками, моторами мощность в мА/ч 
	unsigned long long timeRemaining; // Оставшееся время работы АБК
	unsigned long long timeusec; // Метка времени
};
struct PayloadMessageSysInfo // Сообщения с общими данными о ПЛК
{
	unsigned char mode; //Режим полета (Взлет - 1, Посадка - 2, Возвращаение на точку старта - 3, Автономное выполнение ПЗ - 4
	unsigned char statusMode; //Состояние относительно земли (На змеле (on ground) - 1, Набор высоты (TakingOff) - 2, Полет (In air) - 3, Снижение высоты (Landing) - 4)
	unsigned char navSys; // Текущая используемая система навигации (GPS\ИНС\Optical Flow) ИНС – 1, GPS – 2, Optical Flow – 3
	unsigned char engineStatus; // Текущее состояние моторов. Выключены – 0, включены – 1.
	unsigned char engineCount; // Число моторов БПЛА
	float pressureBaro; //Давление по барометру
	float tempBaro; // Текущая температура барометра
	unsigned long long timeusec; // Метка времени
	unsigned char enginePower[];
};
struct EnginePowerInfo // Структура описывает информацию о двигателе
{
	unsigned char power; //Текущая нагрузка на мотор от 0 до 100%
	unsigned int engineSpeed;//Текущие обороты двигателя (число оборотов в минуту)
	float current; //Текущий ток мотора
	float voltage; //Текущее напряжение мотора
	float temp; //Текущая температура мотора

};
struct PayloadMessageGyroInfo // Сообщение с данными Гироскопа
{
	float yawGyroVel; // Угловая скорость по рысканью (рад\с) (Z)
	float pitchGyroVel; // Угловая скорость по тангажу (рад\с) (Y)
	float rollGyroVel; // Угловая скорость по крену (рад\с) (X)
};
struct PayloadMessageAccelInfo // Сообщение с данными Акселерометра
{
	float yawAccelVel; // Угловая скорость по рысканью (рад\с) (Z)
	float pitchAccelVel; // Угловая скорость по тангажу (рад\с) (Y)
	float rollAccelVel; // Угловая скорость по крену (рад\с) (X)
	float aX; // Ускорение по оси X (м\с)
	float aY; // Ускорение по оси У (м\с)
	float aZ; // Ускорение по оси Z (м\с)
	float tempAccel; //Температура акселерометра
};
struct PayloadMessageGpsInfo // Сообщение с навигационными данными по GPS (частота публикации сообщения – 10 Гц)
{ 
	float lat; //Широта по GPS
	float lon; //Долгота по GPS
	float absAlt; //Абсолютная высота в м
	float realAlt; //Высота относительно земли в м
	float hdop; // Горизонтальная точность сигнала GPS
	float vdop; // Вертикальная точность сигнала GPS
	float pdop; // Точность позиционирования по сигналу GPS
	float noise; // Уровень шума сигнала GPS
	float jamming; // Урвоень глушения сигнала GPS
	unsigned char satVisible; // Число видимых спутников GPS
	unsigned char satUsed; // Число используемых спутников GPS
	float speed; // Скорость по GPS
	unsigned char fixType; // Текущий тип позиционирования. NO_GPS – 0, NO_FIX – 1, 2D_FIX – 2, 3D_FIX – 3, DGPS – 4, RTK_FLOAT – 5, RTK_FIXED – 6, STATIC – 7, PPP – 8
	unsigned long long timeusec; // Метка времени ПЛК
	unsigned long long timeUTC; // Время UTC от GPS спутников
};
struct PayloadMessageInertialInfo // Сообщение с навигационными данными по ИНС (частота публикации сообщения – 10 Гц)
{ 
	float x; // Координата по Х относительно точки взлета
	float y; // Координата по Y относительно точки взлета
	float z; // Координата по Z относительно точки взлета
	float headingDeg; // Текущее направление движения БПЛА по компасу в градусах
	float speed; // Текущая скорость БПЛА по акселерометру в м\с
	float roll; // углы Эйлера БПЛА в градусах относительно плоскостей (от -180 до 180)
	float pitch; // углы Эйлера БПЛА в градусах относительно плоскостей (от -180 до 180)
	float yaw; // углы Эйлера БПЛА в градусах относительно плоскостей (от -180 до 180)
	float rollVel; // Текущие угловые скорости БПЛА в м\с
	float pitchVel; // Текущие угловые скорости БПЛА в м\с
	float yawVel; // Текущие угловые скорости БПЛА в м\с
	float baroAlt; // Высота относительно земли по барометру в метрах
	unsigned long long timeusec; // Метка времени
};
struct PayloadMessageCommandAck // Ответ на любую команду (не запрос сообщения)
{
	unsigned char commandID; // ID команды, на которую отсылается ответ
	unsigned char status; // 0 - команда не может быть выполнена, т.к. ошибка errorCode. 1 - команда принята (errorCode = 0)
	unsigned short errorCode; //id возможной ошибки. Список возможных ошибок необходимо определить заранее. В случае возникновения неизвестной заранее ошибки – значение 255. Ошибок нет – значение 0.
	unsigned long long timeusec; // Метка времени
};
struct PayloadMessageStatusCommand // Сообщение со статусом команды
{
	unsigned long long timeusec; // Метка времени
	unsigned char commandStatus[];
};
struct PayLoadMessageStatusAllCommands // Сообщение со статусами всех командах, котоыре выполняются на ПЛК
{
	unsigned char countCommands; // Число выполняемых команд
	unsigned long long timeusec; // Метка времени
	unsigned char commandsStatus[];
};
struct CommandStatusInfo
{
	unsigned char commandID; // ID команды, на которую отсылается ответ
	unsigned char executionСode; // ошибка при выполнении – 0, команда выполняется – 1, команда выполнена успешно – 2
	unsigned short errorCode; //id возможной ошибки. Список возможных ошибок необходимо определить заранее. В случае возникновения неизвестной заранее ошибки – значение 255. Ошибок нет – значение 0.
};
struct PayloadCommandRequestMessage
{
	unsigned char commandID; // ID команды
	unsigned char reqMessageId; //ID сообщение, которое нужно получить в ответ на запрос
	unsigned long long timeusec; // Метка времени
};
struct PayloadCommandRequestStatusCommand
{
	unsigned char commandID; // ID команды
	unsigned char ID; // ID команды, статус которой нужно получить
	unsigned long long timeusec; // Метка времени
};
struct PayloadCommandChangeNav // Команда о смене системы навигации
{ 
	unsigned char commandID; // ID команды
	unsigned char navSys; // Требуемая система навигации (GPS\ИНС\Optical Flow). ИНС – 1, GPS – 2, Optical Flow – 3
	unsigned long long timeusec; // Метка времени КК
};
struct PayLoadCommandChangeSpeed // Команда о измении скорости полета
{ 
	unsigned char commandID; // ID команды
	float requiredSpeed; // Требуемая скорость полета в м\с
	unsigned long long timeusec; // Метка времени КК
};
struct PayLoadCommand // Общая структура некоторых команд (land,pause,startEngine и т.д.)
{ 
	unsigned char commandID; // ID команды
	unsigned long long timeusec; // Метка времени КК
};
struct PayloadCommandGoTo // Команда с очередной точкой полетного задания
{ 
	unsigned char commandID; // ID команды
	float lat; // Широта
	float lon; // Долгота
	float alt; // Высота относительно земли
	float headingDeg; // Направление по компасу, в сторону которого будет повернут дрон во время движения к точке в градусах
	float speed; // Скорость БПЛА при движении к точке (может быть изменена командой об изменении скорости) в м\с
	unsigned long long timeusec; // Метка времени КК
};
#pragma pack (pop)

int main()
{
	unsigned char eng[4] = { 10, 10, 10, 10};
	unsigned short crc = 0;
	crc += (unsigned char)1;
	crc += (unsigned char)1;
	crc += (unsigned char)1;
	crc += (unsigned char)1;
	crc += (unsigned long long)1001231231;
	crc += (unsigned char)4;
	for (int i = 0; i < 4; i++) 
	{
		crc += (unsigned char)eng[i];
	}
	//Сообщение
	unsigned char buffer[sizeof(Header) + sizeof(PayloadMessageSysInfo) + (4 * sizeof(EnginePowerInfo))];
	Header* bufHead = (Header*)buffer;
	bufHead->stx = 0xAA;
	bufHead->msgId = MessagesId::SysInfo;
	bufHead->lenPayload = sizeof(PayloadMessageSysInfo) + (4 * sizeof(EnginePowerInfo));
	bufHead->crc = crc;
	PayloadMessageSysInfo* bufSysInfo = (PayloadMessageSysInfo*)(buffer + sizeof(Header));
	bufSysInfo->mode = 1;
	bufSysInfo->statusMode = 1;
	bufSysInfo->navSys = 1;
	bufSysInfo->engineStatus = 1;
	bufSysInfo->timeusec = 1001231231;
	bufSysInfo->engineCount = 4;
	EnginePowerInfo* bufEngin = (EnginePowerInfo*)(buffer + sizeof(Header) + sizeof(PayloadMessageSysInfo));
	for (int i = 0; i < bufSysInfo->engineCount; i++)
	{
		bufEngin[i].power = eng[i];
	}
	return 0;
};
