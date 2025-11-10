// CODE BY TRẦN VỸ

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// --- CẤU HÌNH WIFI VÀ SERVER ---
ESP8266WebServer server(80);
IPAddress local_ip(192, 168, 49, 15);
IPAddress gateway(192, 168, 49, 1);
IPAddress subnet(255, 255, 255, 0);

// --- CẤU HÌNH CHÂN GPIO ---
#define ENA D5   // Enable/Speed motors Right        GPIO14
#define ENB D6   // Enable/Speed motors Left         GPIO12
#define IN_1 D8  // L298N in1 motors Right           GPIO15
#define IN_2 D7  // L298N in2 motors Right           GPIO13
#define IN_3 D4  // L298N in3 motors Left            GPIO2
#define IN_4 D3  // L298N in4 motors Left            GPIO0
#define LED D2   // LED HIỆU ỨNG

bool Tien = LOW;
bool Lui = LOW;
bool Phai = LOW;
bool Trai = LOW;
bool Stop = LOW;
bool TraiTien = LOW;
bool TraiLui = LOW;
bool PhaiTien = LOW;
bool PhaiLui = LOW;
bool HieuUng1 = LOW;
bool HieuUng2 = LOW;
bool HieuUng3 = LOW;

// --- BIẾN TOÀN CỤC ---
int speedCarA = 900;  // tốc độ PWM (0 - 1023)
int speedCarB = 1023;
int speed_Coeff = 9;

// --- GIAO DIỆN WEB ---
const char* HTML_CONTENT = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
	 <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
	<title>Điều khiển thiết bị</title>
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<style>
		.b{width: 130px;height: 50px;font-size: 25px;color: #FFF;background-color:#4caf50;border-radius: 10px;}
		.t{width: 130px;height: 50px;font-size: 25px;color: #FFF;background-color:#f44336;border-radius: 10px;}
		.n{width: 130px;height: 50px;font-size: 25px;color: #FFF;background-color:#2B60DE;border-radius: 10px;}
		.i{width: 130px;height: 50px;font-size: 25px;color: #FFF;background-color:#C12869;border-radius: 10px;}
		.h{width: 130px;height: 50px;font-size: 21px;color: #FFF;background-color:#FF6600;border-radius: 10px;}
	</style>
</head>
<body>
<div style="width: 420px;height: auto;margin: 0 auto;margin-top: 40px">
<h1 align="left">🚗CONTROL CAR BY WIFI</h1>
<h2 align="center">DESIGN BY TRẦN VỸ</h2>
	<table align="right" cellpadding="5" >   
		<tr>
			<td><a href='/tientrai'><button class='i'>Tiến trái</button></a><td></td>
			<td><a href='/tien'><button class='n'>Tiến</button></a><td>		
			<td><a href='/tienphai'><button class='i'>Tiến phải </button></a><td></td>
	    <tr>
	    <tr>
			<td><a href='/trai'><button class='b'>Trái</button></a><td></td>
			<td><a href='/STOP'><button class='t'>STOP</button></a><td>
			<td><a href='/phai'><button class='b'>Phải</button></a><td></td>
	    <tr>
			<td><a href='/luitrai'><button class='i'>Lùi trái</button></a><td></td>
			<td><a href='/lui'><button class='n'>Lùi</button></a><td></td>
			<td><a href='/luiphai'><button class='i'>Lùi phải</button></a><td></td>
	    <tr>
		<tr>
			<td><a href='/hieuung1'><button class='h'>Hiệu ứng 1</button></a><td></td>
			<td><a href='/hieuung2'><button class='h'>Hiệu ứng 2</button></a><td></td>
			<td><a href='/hieuung3'><button class='h'>Hiệu ứng 3</button></a><td></td>
		</tr>		
	</table>
</div>
</body>
</html>
)rawliteral";

// --- HÀM ĐIỀU KHIỂN ĐỘNG CƠ ---
void goAhead() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCarA);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCarB);
}

void goBack() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCarA);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCarB);
}

void goRight() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCarA / speed_Coeff);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCarB / speed_Coeff);
}

void goLeft() {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCarA / speed_Coeff);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCarB / speed_Coeff);
}
void goAheadRight() {

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCarA);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCarB / speed_Coeff);
}

void goAheadLeft() {

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCarA / speed_Coeff);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCarB);
}

void goBackRight() {

  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCarA);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCarB / speed_Coeff);
}

void goBackLeft() {

  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCarA / speed_Coeff);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCarB);
}

void stopRobot() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, 0);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, 0);
}
void LEDMAGIC1() {
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
}

// --- CÁC HÀM XỬ LÝ WEB ---
void event_OnConnect() {
  Tien = Lui = Phai = Trai = Stop = LOW;
  stopRobot();
  server.send(200, "text/html", HTML_CONTENT);
  Serial.println("➡️ Trang chủ được truy cập");
}

void event_NotFound() {
  server.send(404, "text/html", HTML_CONTENT);
}

void TIEN() {
  Tien = HIGH;
  Lui = Phai = Trai = Stop = TraiTien = TraiLui = PhaiTien = PhaiLui = LOW;
  goAhead();
  server.send(200, "text/html", HTML_CONTENT);
  Serial.println("🚗 Tiến");
}

void LUI() {
  Lui = HIGH;
  Tien = Phai = Trai = Stop = TraiTien = TraiLui = PhaiTien = PhaiLui = LOW;
  goBack();
  server.send(200, "text/html", HTML_CONTENT);
  Serial.println("🔙 Lùi");
}

void PHAI() {
  Phai = HIGH;
  Tien = Lui = Trai = Stop = TraiTien = TraiLui = PhaiTien = PhaiLui = LOW;
  goRight();
  server.send(200, "text/html", HTML_CONTENT);
  Serial.println("↪️ Rẽ phải");
}

void TRAI() {
  Trai = HIGH;
  Tien = Lui = Phai = Stop = TraiTien = TraiLui = PhaiTien = PhaiLui = LOW;
  goLeft();
  server.send(200, "text/html", HTML_CONTENT);
  Serial.println("↩️ Rẽ trái");
}
void PHAITIEN() {
  PhaiTien = HIGH;
  Tien = Lui = Phai = Trai = TraiTien = TraiLui = Stop = PhaiLui = LOW;
  goAheadRight();
  server.send(200, "text/html", HTML_CONTENT);
  Serial.println("Chạy thẳng sang phải");
}
void PHAILUI() {
  PhaiLui = HIGH;
  Tien = Lui = Phai = Trai = TraiTien = TraiLui = Stop = PhaiTien = LOW;
  goBackRight();
  server.send(200, "text/html", HTML_CONTENT);
  Serial.println("Lùi xe sang phải");
}
void TRAITIEN() {
  TraiTien = HIGH;
  Tien = Lui = Phai = Trai = PhaiTien = TraiLui = Stop = PhaiLui = LOW;
  goAheadLeft();
  server.send(200, "text/html", HTML_CONTENT);
  Serial.println("Chạy thẳng sang phải");
}
void TRAILUI() {
  TraiLui = HIGH;
  Tien = Lui = Phai = Trai = PhaiTien = TraiTien = Stop = PhaiLui = LOW;
  goBackLeft();
  server.send(200, "text/html", HTML_CONTENT);
  Serial.println("Chạy thẳng sang phải");
}

void STOP() {
  Stop = HIGH;
  Tien = Lui = Phai = Trai = TraiTien = TraiLui = PhaiTien = PhaiLui = LOW;
  stopRobot();
  server.send(200, "text/html", HTML_CONTENT);
  Serial.println("🛑 Dừng lại");
}
void THEDIFT(){
  HieuUng1= HIGH;
  goAhead();
  delay(2000);
  goRight();
  delay(500);
  LEDMAGIC1();
  stopRobot();
  server.send(200, "text/html", HTML_CONTENT);
  Serial.print("TOKYO THE DIFT");
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n--- Khởi động ESP8266 ---");

  // Thiết lập chân xuất
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(LED,OUTPUT);

  // --- TẠO WIFI RIÊNG CHO XE ---
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Xe_DieuKhien", "12345678");
  WiFi.softAPConfig(local_ip, gateway, subnet);

  Serial.println("✅ WiFi AP đã tạo thành công!");
  Serial.print("📶 SSID: Xe_DieuKhien\n🔑 Password: 12345678\n");
  Serial.print("🌐 Truy cập bằng IP: ");
  Serial.println(WiFi.softAPIP());

  // Cấu hình các đường dẫn (Routes)
  server.on("/", event_OnConnect);
  server.on("/tien", TIEN);
  server.on("/lui", LUI);
  server.on("/phai", PHAI);
  server.on("/trai", TRAI);
  server.on("/tientrai", TRAITIEN);
  server.on("/tienphai", PHAITIEN);
  server.on("/luiphai", PHAILUI);
  server.on("/luitrai", TRAILUI);
  server.on("/STOP", STOP);
  server.on("/hieuung1",THEDIFT);
  server.onNotFound(event_NotFound);

  server.begin();
  Serial.println("🌎 Web server đã khởi động!");
}

void loop() {
  server.handleClient();
}
