# Flappy Bird - Project Game

(1) Thông tin cá nhân

- Họ và tên: Nguyễn Hồng Hoàng
- Mã sinh viên: 24020139

## Giới thiệu chủ đề game

Flappy Bird là một game 2D đơn giản được xây dựng bằng SDL2 và C++, tái hiện phiên bản gốc với mục tiêu điều khiển chú chim vượt qua các cột trụ để ghi điểm. Game có cải tiến và thêm vài tính năng cơ bản như tạm dừng, tăng tốc độ, lưu điểm cao, cùng với hiệu ứng đồ họa và âm thanh sống động.

## Ngưỡng điểm mong muốn

Tôi cho rằng project này xứng đáng đạt mức điểm 9-10 theo barem được cung cấp.

## Lý do bảo vệ ngưỡng điểm

Dựa trên barem điểm:

### 4-6: Học và làm game theo nội dung hướng dẫn hàng tuần của giảng viên, nộp bài đúng hạn trên GitHub

- Tôi đã hoàn thành game Flappy Bird theo hướng dẫn hàng tuần, bao gồm các tính năng cơ bản như điều khiển chim, vượt cột, ghi điểm, và kết thúc game.
- Project đã được nộp đúng hạn trên GitHub với đầy đủ mã nguồn và tài nguyên.

### 6.5-7:

1. **Game tự viết với thuật toán đơn giản và không đồ họa**:

   - Game có thuật toán đơn giản: điều khiển chim bằng trọng lực, di chuyển cột trụ, phát hiện va chạm, và tăng tốc độ dựa trên số cột vượt qua.
   - Tuy nhiên, game của tôi có đồ họa đầy đủ, nên vượt yêu cầu này.

2. **Lấy cơ số từ code được hướng dẫn và phát triển thêm một chút**:
   - Tôi đã phát triển thêm các tính năng: trạng thái tạm dừng (PAUSED), tăng tốc độ sau mỗi cột, hiển thị chữ "Paused" với màu vàng đậm, lưu điểm cao vào file.

### 7.5-8.5:

1. **Game tự viết có thuật toán nâng cao ngoài chương trình môn học**:

   - Thuật toán tăng tốc độ: Tốc độ cuộn của cột trụ tăng thêm 1 đơn vị sau mỗi cột vượt qua, làm game khó hơn theo thời gian.
   - Quản lý trạng thái game (MENU, PLAYING, PAUSED, GAME_OVER) với các sự kiện tương ứng (nhấn Space, P).

2. **Game tự viết có đồ họa đơn giản**:

   - Game có đồ họa đầy đủ: hình ảnh cho chim, cột, nền, mặt đất, nút Start, Game Over, Restart; hiển thị điểm số và điểm cao bằng font chữ.

3. **Game phát triển từ code mẫu nhưng có thay đổi đáng kể**:
   - Tôi đã thay đổi đáng kể so với code mẫu: thêm trạng thái PAUSED, tăng tốc độ động, hiển thị chữ "Paused" với màu vàng đậm, lưu điểm cao vào file "highscore.txt".

### 9-10: Chất lượng ở top 10% của lớp

- **Điểm nổi bật**:
  - **Tính năng tạm dừng**: Nhấn phím P để tạm dừng game, hiển thị chữ "Paused" màu vàng đậm (RGB: 255, 215, 0), nhấn P lần nữa để tiếp tục.
  - **Tăng tốc độ động**: Tốc độ cột trụ tăng sau mỗi cột, làm game khó hơn theo thời gian.
  - **Hiệu ứng đồ họa và âm thanh**: Chim dao động ở MENU, màn hình rung khi va chạm, âm thanh vỗ cánh, va chạm, ghi điểm, và chết.
  - **Tối ưu code**: Sử dụng "ResourceManager" để quản lý tài nguyên hiệu quả, giảm lặp code khi hiển thị text bằng hàm "renderText".
  - **Lưu điểm cao**: Điểm cao nhất được lưu vào file "highscore.txt" và hiển thị ở màn hình GAME_OVER.

## Các nguồn tham khảo

- **Tutorial SDL2**:
  - [Lazy Foo' Productions - SDL Tutorials](https://lazyfoo.net/tutorials/SDL/): Hướng dẫn cơ bản về SDL2, cách khởi tạo, vẽ hình, xử lý sự kiện.
  - [YouTube - SDL2 Game Development Tutorials](https://www.youtube.com/playlist?list=PLhfAbcv9cehhkG7ZQK0dol2yDzO1YPlt): Video hướng dẫn lập trình game với SDL2.
- **Tài nguyên đồ họa và âm thanh**:
  - Hình ảnh (background.png, bird.png, pipe_end.png, pipe_body.png, v.v.): Tải từ https://github.com/TimoWilken/flappy-bird-pygame/tree/master/images, internet và chỉnh sửa để phù hợp.
  - Âm thanh (flap.wav, hit.wav, die.wav, swoosh.wav, point.wav): Tải từ internet.
  - Font chữ (arial.ttf, arialbd.ttf): Có sẵn trong windows.
- **Tham khảo code mẫu**:
  - Code mẫu từ hướng dẫn của giảng viên (cung cấp trong lớp học), được dùng làm nền tảng và phát triển thêm.

## Mức độ sử dụng AI

- Tôi đã sử dụng AI để hỗ trợ trong quá trình viết code, với các mục đích sau:
  - Hướng dẫn sửa lỗi: Ví dụ, sửa lỗi tải tài nguyên (pause.png), lỗi truy cập bộ nhớ (Access Violation).
  - Gợi ý cải tiến: Thêm tính năng tạm dừng, tăng tốc độ, hiển thị chữ "Paused" với màu vàng đậm.
  - Tối ưu code: Gợi ý viết hàm "renderText" để giảm lặp code khi hiển thị text.
- Tuy nhiên, tôi tự học và tự viết phần lớn code, chỉ dùng AI để giải đáp thắc mắc và tối ưu, không sao chép toàn bộ code từ AI.

## Hướng dẫn chạy game

1. Cài đặt SDL2, SDL_image, SDL_ttf, SDL_mixer trên máy tính.
2. Clone repository này về máy: https://github.com/NguyenHongHoang10/Make_Game_LTNC.
3. Mở project trong IDE (như Visual Studio, CodeBlocks).
4. Đảm bảo các file tài nguyên (hình ảnh, âm thanh, font) nằm trong thư mục gốc của project.
5. Biên dịch và chạy file `main.cpp`.

## Cấu trúc project

- `Constants.h`: Hằng số làm thông số cho game (kích thước màn hình, tốc độ, v.v.).
- `GameState.h`: Enum trạng thái game.
- `ResourceManager.h/cpp`: Quản lý tài nguyên.
- `Utils.h/cpp`: Hàm tiện ích.
- `Bird.h/cpp`: Lớp quản lý chú chim.
- `Pipe.h/cpp`: Lớp quản lý cột trụ.
- `GameManager.h/cpp`: Lớp chính điều khiển game.
- `main.cpp`: File chính chạy game
  Make_Game_LTNC
