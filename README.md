# STM32F103C8T6: đo nhiệt độ và độ ẩm

Firmware đọc cảm biến DHT11 và hiển thị nhiệt độ, độ ẩm lên LCD ký tự 16×2 ở chế độ song song 8-bit. Dự án dùng STM32 HAL, cấu hình STM32CubeMX và GNU Arm Embedded Toolchain.

## Phần cứng

- STM32F103C8T6 (board tương thích Blue Pill hoặc phần cứng tương đương)
- Cảm biến DHT11
- LCD ký tự 16×2 tương thích HD44780, giao tiếp 8-bit
- GNU Make và GNU Arm Embedded Toolchain để biên dịch
- Node.js nếu muốn chạy mô phỏng hiển thị trong `tests/`

## Kết nối

| Tín hiệu | Chân STM32 | Ghi chú |
| --- | --- | --- |
| LCD D0–D7 | PA0–PA7 | Bus dữ liệu 8-bit |
| LCD RS | PB0 | Chọn lệnh/dữ liệu |
| LCD RW | PB1 | Firmware giữ ở chế độ ghi (RW = 0) |
| LCD EN | PB10 | Enable |
| DHT11 DATA | PB11 | Đường dữ liệu một dây |
| Nguồn, GND | Theo module | Nối chung GND; kiểm tra mức logic trước khi nối |

Dùng điện trở kéo lên cho DATA của DHT11 nếu module chưa tích hợp. Đảm bảo điện áp trên các chân LCD tương thích mức logic 3,3 V của STM32 trước khi nối trực tiếp.

## Hoạt động

Sau khi khởi tạo clock, GPIO, TIM2 và LCD, firmware đọc DHT11 định kỳ khoảng 2 giây. Hai dòng LCD hiển thị `TEMP` và `HUM`; nếu đọc cảm biến thất bại, màn hình hiển thị `ERROR`. `Core/Src/display.c` định dạng dữ liệu thành hai dòng 16 ký tự và đệm khoảng trắng để xóa ký tự còn sót từ lần hiển thị trước.

Clock ngoài HSE 8 MHz được nhân PLL ×9 thành SYSCLK 72 MHz. APB1 chạy ở 36 MHz; timer APB1 chạy ở 72 MHz. TIM2 có prescaler 71 để tạo bộ đếm xấp xỉ 1 MHz, dùng bởi các hàm delay trong `Core/Src/main.c` và giao tiếp DHT11.

## Biên dịch

Cần có `arm-none-eabi-gcc`, `arm-none-eabi-objcopy`, `arm-none-eabi-size` và GNU Make trong `PATH`. Chạy từ thư mục gốc:

```sh
make
```

Nếu toolchain ở thư mục riêng, truyền đường dẫn đến thư mục `bin`:

```sh
make GCC_PATH=/path/to/toolchain/bin
```

Trên Windows cần môi trường cung cấp GNU Make và shell hiểu các lệnh `mkdir`/`rm` của Makefile, chẳng hạn MSYS2. Sản phẩm được tạo trong `build/`:

- `temp.elf`: firmware có thông tin debug
- `temp.hex`: ảnh Intel HEX
- `temp.bin`: ảnh nhị phân

Dọn sản phẩm biên dịch bằng `make clean`.

## Mô phỏng hiển thị

Chạy harness tùy chọn bằng Node.js:

```sh
node tests/lcd_simulation.mjs
```

Harness kiểm tra một số cấu trúc mã nguồn bằng biểu thức chính quy, sau đó mô phỏng riêng cách ghi các chuỗi lên LCD 16×2 với dữ liệu mẫu, lỗi đọc và cập nhật lặp lại. Nó không biên dịch/chạy `Display_FormatRows()` trực tiếp, cũng không mô phỏng timing GPIO, giao thức DHT11 hay phần cứng LCD.

## Cấu trúc thư mục

```text
Core/                 Mã ứng dụng, driver DHT11/LCD và cấu hình HAL
Drivers/              STM32 HAL và CMSIS
tests/                Harness mô phỏng nội dung hiển thị
build/                Sản phẩm biên dịch
Makefile              Quy tắc build GNU Arm
temp.ioc              Cấu hình STM32CubeMX
startup_stm32f103xb.s Vector ngắt và mã khởi động
STM32F103xx_FLASH.ld  Bố trí Flash/RAM khi liên kết
```

Linker script hiện cấu hình 64 KiB Flash và 20 KiB RAM. Hãy xác nhận biến thể MCU và kích thước bộ nhớ của board trước khi nạp firmware.
