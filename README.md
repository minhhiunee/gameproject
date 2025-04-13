1. Ý tưởng:
   - Phần đầu (Running): game chạy, vượt chướng ngại vật tựa tựa dino game, cộng điểm khi vượt qua vật cản.
   - Phần sau (Flying): game lái máy bay, bắn kẻ địch, tính điểm và thắng khi đủ điểm.

   - Ý tưởng phát triển:
   
                         + Running: buff tăng tốc độ, bất tử, khiên.
                         + Flying: buff tăng tốc độ, bất tử, khiên (nhận được bằng viện tiêu diệt kẻ thù). Tạo thêm boss để end game.
   

2. Quá trình làm game:
   - 12/03/2025:
   
                 + Xây dựng khung sườn cơ bản của game. Hoạt họa ban đầu của nhân vật, vật cản, kẻ địch và đạn bắn tạm thời đơn giản là hình ô vuông.
                 + Các nút cơ bản: Space: Nhảy (phần đầu)
                                          Bắn (phần sau).
                                   Key ⬆️ and ⬇️: di chuyển lên xuống (phần sau).
   - 15/03/2025:
   
                + Tách hàm của nhân vật, vật cản, kẻ địch và đạn thành các module riêng .h và .cpp. (có sử dụng AI trợ giúp)
   - 20/03/2025:
   
                 + Sử dụng kĩ thuật Scrolling Background cho chế độ Running - nguồn https://lazyfoo.net/tutorials/SDL/31_scrolling_backgrounds/index.php.
   
   - 09/04/2025:
   
                 + Sử dụng kĩ thuật Scrolling Background cho chế độ Flying.
                 + Thêm ảnh cho các lớp Player, Enenmy, Object và Bullet.
                 + Up ảnh cho các lớp Player, Enenmy, Object và Bullet.
                 + Tạo hàm random ảnh cho việc spawn object (running) và enemy (flying).
                 + Xử lý lại kích thước width và height của lớp Player.
                 + Xử lý lại logic va chạm của cả 2 chế độ.
                 + Tạo thêm file main.h để quản lý chế độ chơi dễ hơn.
   - 13/04/2025:
     
                 + Thêm thuộc tính máu vào lớp Player.
                 + Thêm khả năng bắn cho Enemy.
                 + Thêm và up ảnh vụ nổ của Enemy khi bị bắn trúng.
                 + Thêm khả năng bắn từ 2 phía cho lớp Player.
                 + Thêm quản lý đạn sao cho phù hợp về thời gian giữa các lần bắn.
                 + Xử lý lại logic va chạm giữa đạn với player và enemy.
     
   
