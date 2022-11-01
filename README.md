# XuLiAnh01
Chương trình xử lí ảnh cơ bản (Chỉnh sửa kích thước, lật ảnh)

Chương trình được viết có các chức năng sau:
		- Phóng to ảnh.
		- Thay đổi kích thước ảnh.
		- Đảo ngược ảnh theo chiều dọc.
		- Đảo ngược ảnh theo chiều ngang.
  

Hướng dẫn sử dụng: Nhập lần lượt theo thứ tự: “file.exe” “mã lệnh” “mã lệnh” “giá trị” “giá trị” “ảnh input” ‘ảnh output’  như sau (Mỗi mã lệnh sẽ có tham số cần nhập khác nhau).

Phóng to ảnh.

<Program.exe> -zoom <interp> <sx> <sy> <InputFilePath> <OutputFilePath>

• Program.exe: the name of executable file
• InputFilePath: the path name of the input file
• OutputFilePath: the path name of the output file
• -zoom: the command name
• sx: horizontal scale coefficient
• sy: vertical scale coefficient
• interp: interpolation method (using ”-bl” for bilinear interpolation or ”-nn” for nearest neighbor)

Thay đổi kích thước ảnh.

<Program.exe> -resize <interp> <nw> <nh> <InputFilePath> <OutputFilePath>

• Program.exe: the name of executable file
• InputFilePath: the path name of the input file
• OutputFilePath: the path name of the output file
• -resize: the command name
• nw: new width
• nh: new height
• interp: interpolation method (using ”-bl” for bilinear interpolation or ”-nn” fornearest neighbor)

Đảo ngược ảnh theo chiều dọc.

<Program.exe> -flipV <interp> <InputFilePath> <OutputFilePath>

• Program.exe: the name of executable file
• InputFilePath: the path name of the input file
• OutputFilePath: the path name of the histogram image file
• -flipV : the command name
• interp: interpolation method (using ”-bl” for bilinear interpolation or ”-nn” for nearest neighbor)

Đảo ngược ảnh theo chiều ngang.

<Program.exe> -flipH <interp> <InputFilePath> <OutputFilePath>

• Program.exe: the name of executable file
• InputFilePath: the path name of the input file
• OutputFilePath: the path name of the output file
• -flipH : the command name
• interp: interpolation method (using ”-bl” for bilinear interpolation or ”-nn” for nearest neighbor) 


