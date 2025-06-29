# 🌟 Chào mừng bạn đến với thiên đường ESP 🌟

Đây là một dự án nhỏ sử dụng **ESP32** nhằm **phát hiện rò rỉ khí Gas hoặc cháy nổ** và **kích hoạt hệ thống cảnh báo & thoát hiểm**, đồng thời **gửi cảnh báo qua ứng dụng Blynk IoT**.

---

## ⚙️ Nguyên lý hoạt động

- **Cảm biến MQ2** đo nồng độ khí Gas và gửi dữ liệu về **ESP32** cũng như lên **Blynk IoT**.
- Khi phát hiện khí vượt ngưỡng:
  - **Buzzer và LED** được kích hoạt cảnh báo.
  - **Servo** mở cửa thoát hiểm.
  - **Relay** điều khiển **quạt thông gió** để hút khí độc (Gas, CO, CO₂) ra ngoài.
  - Thông báo gửi về ứng dụng Blynk.
- Có **công tắc tay** để mở cửa thoát hiểm thủ công khi cần thiết.

---

## 🛠️ Các bước thực hiện

### ✅ Bước 1: Chuẩn bị linh kiện
Xem chi tiết trong file: `linhkien.pdf`

### ✅ Bước 2: Kết nối phần cứng
Làm theo sơ đồ nguyên lý & PCB trong file: `Sodonguyenly_PCB.pdf`

### ✅ Bước 3: Nạp chương trình cho ESP32
- Mở file `Canh_bao_va_thoat_hiem.ino`
- Cập nhật:
  - `WiFi SSID` và `Password`
  - `Blynk Template ID`, `Device Name`, `Auth Token`
- Nạp vào ESP32 qua Arduino IDE

### ✅ Bước 4: Thiết lập Blynk IoT

1. **New Template**: Tạo Template mới theo hình minh họa
   
   ![image](https://github.com/user-attachments/assets/297d1e0b-53da-456b-8ff5-e7e6e7f1bb9b)

3. **Datastreams**: Tạo các dòng dữ liệu (Vx) giống như hình hướng dẫn

   ![image](https://github.com/user-attachments/assets/625943b9-8f4c-4088-9cec-870dcbb3423a)

5. **Web Dashboard**: Thiết lập giao diện điều khiển

   ![image](https://github.com/user-attachments/assets/785aa85a-8301-433e-b932-60d86c44c858)

   ![image](https://github.com/user-attachments/assets/99bba294-bf20-4027-9344-f87c8109bae3)

7. **Devices**: 
   - `New Device` → Chọn Template đã tạo → `Create`
8. **Automation**:
   - Vào tab `Automation` → `Create Automation` → `Device state` → cấu hình như hình

  ![image](https://github.com/user-attachments/assets/cfccc6f2-abfb-4194-9ec9-17110b6c6728)
  
  ![image](https://github.com/user-attachments/assets/44797f6e-52cb-432e-92db-96cf3bb03130)

### ✅ Bước 5: Tận hưởng thành quả!
- Khi phát hiện khí Gas hoặc cháy, hệ thống sẽ cảnh báo tự động và kích hoạt thoát hiểm.
- Bạn có thể giám sát và điều khiển mọi thứ từ ứng dụng **Blynk IoT** trên điện thoại hoặc web.

---

## 📎 Ghi chú

- **Hãy chắc chắn đã thay đổi WiFi & Auth Token trong code** trước khi nạp.

---

## 🎥 Video Demo

Nếu bạn có ý tưởng cải tiến hoặc phát hiện lỗi, hãy tạo issue hoặc pull request trên GitHub!

---
