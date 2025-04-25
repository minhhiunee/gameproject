# Game project description
## Mục lục

### [I. Ý tưởng triển khai](#ytuong)
- [1. Nguồn tham khảo game:](#nguonthamkhaogame)
- [2. "Cốt truyện" chính của game](#cottruyen)
### [II. Cách tải](#cachtai)
	
### [III. Cách chơi](#cachchoi)
- [Điều khiển](#dieukhien)
- [Chức năng chính](#chucnang)
### [III. Nguồn tài nguyên tham khảo](#tainguyen)
- [Đồ họa](#dohoa)
- [Source code](#code)

<a name="ytuong"></a>
## I.Ý tưởng triển khai:
<a name="nguonthamkhaogame"></a>
1. Nguồn tham khảo game:
   - Nguồn game: tham khảo từ tựa game dino run và game bắn máy bay cổ điển.
   - Phần đầu (Running): game chạy, vượt chướng ngại vật tựa tựa dino game, cộng điểm khi vượt qua vật cản.
   - Phần sau (Flying): game lái máy bay, bắn kẻ địch, tính điểm và thắng khi đủ điểm.
<a name="cottruyen"></a>
2. "Cốt truyện" chính của game:
    bạn là một kẻ nghiện ngập ở Trái Đất và bị người ngoài hành tinh bắt đến một nơi kì lạ. Việc của bạn là chạy đến một kho chứa máy bay của chúng, đánh cắp một chiếc và trốn thoái khỏi hành tinh này. Nhưng để làm được việc đó, bạn phải vượt qua các chướng ngại vật trên đường đi và đội quân bảo vệ bầu trời của bọn chúng. Chúc may mắn !!
<a name="cachtai"></a>
## II.Cách tải: (bổ sung sau)

<a name="cachchoi"></a>
## III. Cách chơi:
<a name="dieukhien"></a>
1. Điều khiển:
   - Chế độ RUNNING: ấn `SPACE  ` để nhảy.
   - Chế độ FLYING: Key ⬆️, ⬇️, ⬅️, ➡️ di chuyển lên, xuống, trái, phải.
<a name="chucnang"></a>
2. Chức năng chính:
   - Space: nhảy (RUNNING) và bắn (FLYING).
   
<a name="tainguyen"></a>
## IV: Nguồn tài nguyên tham khảo:
<a name="dohoa"></a>
1. Đồ họa:
<a name="code"></a>
2. Source code:

## V. Quá trình làm game:
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
   - 15/04/2025:
   
                 + Cài đặt SDL2_ttf.
                 + Tạo giao diện UI gồm GameStart và GameOver.
                 + Thêm nút Start và nút Restart.
                 + Hiển thị điểm đạt được, thêm thanh máu cho nhân vật ở chế độ FLYING.
   - 20/04/2025:

                 + Tăng độ khó cho game bằng cách tăng tốc độ chạy của nhân vật khi đạt mức điểm +=50.
                 + Điều chỉnh lại hoạt ảnh chạy của nhân vật.
                 + Điều chỉnh lại Object.
   - 24/04/2025: Big updates:

                 + Thêm chức năng di chuyển 4 hướng: lên, xuống, trái, phải.
                 + Thêm buff ngẫu nhiên mỗi mốc 50 điểm: Tăng tốc độ, bất tử và hồi máu (10 giây - 2 máu / giây).
                 + Thêm màn hình Victory ( điều kiện thắng là điểm >= 1000 điểm ).
                 + Sửa lại UI.h và Ui.cpp để hiện buff đang có.
                 + Tăng độ khó cho game bằng cách: tăng tốc độ spawn Enemy, tăng tốc độ bắn của Enemy sau mỗi mốc 50 điểm trong chế độ Flying.
                 + Thêm điều kiện Game Over: nếu bị miss và để Enemy đi ra ngoài màn hình hoặc đâm vào Player.
                 + Cập nhật lớp Player.
                 + Cập nhật lớp Object ( giảm thời gian spawn vật cản sau mỗi mốc 50 điểm ).
                 + ....
                 
     
     
   
