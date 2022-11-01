#include "GeometricTransformer.h"
/*
 Lớp base dùng để nội suy màu của 1 pixel
*/
PixelInterpolate::PixelInterpolate(){}
PixelInterpolate::~PixelInterpolate(){}

/*
Lớp nội suy màu theo phương pháp song tuyến tính
*/
uchar BilinearInterpolate::Interpolate(float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels, int iChannel_dst)
{
	int x0 = round(tx), y0 = round(ty);
	float a = tx - x0, b = ty - y0;

	//Tính các giá trị lân cận
	//Tọa dộ điểm lân cận (tx, ty) tại (x0, y0) 
	uchar* pSrcRow_0 = pSrc + (x0 * srcWidthStep + y0 * nChannels);
	//Tọa dộ điểm lân cận (tx, ty) tại (x0 + 1, y0) 
	uchar* pSrcRow_1 = pSrc + ((x0 + 1) * srcWidthStep + y0 * nChannels);
	//Tọa dộ điểm lân cận (tx, ty) tại (x0, y0 + 1) 
	uchar* pSrcRow_2 = pSrc + (x0 * srcWidthStep + (y0 + 1) * nChannels);
	//Tọa dộ điểm lân cận (tx, ty) tại (x0 + 1, y0 + 1) 
	uchar* pSrcRow_3 = pSrc + ((x0 + 1) * srcWidthStep + (y0 + 1) * nChannels);

	//Phép nội suy giá trị màu của điểm ảnh:
	//f'(tx,ty) = (1 - a)(1 - b)f(x0,y0) 
	// + a(1 - b)f(x0 + 1,y0) 
	// + b(1 - a)f(x0,y0 + 1) 
	// + abf(x0 + 1, y0 + 1)
	return saturate_cast<uchar>((1 - a) * (1 - b) * pSrcRow_0[iChannel_dst]
			+ a * (1 - b) * pSrcRow_1[iChannel_dst]
			+ b * (1 - a) * pSrcRow_2[iChannel_dst]
			+ a * b * pSrcRow_3[iChannel_dst]);
}

BilinearInterpolate::BilinearInterpolate(){}
BilinearInterpolate::~BilinearInterpolate(){}

/*
Lớp nội suy màu theo phương pháp láng giềng gần nhất
*/
uchar NearestNeighborInterpolate::Interpolate(float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels, int iChannel_dst)
{
	int x0 = (int)tx, y0 = (int)ty;
	uchar* pSrcRow = pSrc + x0 * srcWidthStep + y0 * nChannels;

	return pSrcRow[iChannel_dst];
}

NearestNeighborInterpolate::NearestNeighborInterpolate(){}
NearestNeighborInterpolate::~NearestNeighborInterpolate(){}

/*
Lớp biểu diễn pháp biến đổi affine
*/
void AffineTransform::Translate(float dx, float dy)
{
	//Tạo ma trận tịnh tiến theo công thức:
	// (1,0,dx)
	// (0,1,dy)
	// (0,0,1)

	//Tạo 1 ma trận tạm để tính toán
	Mat Matrix = Mat(3, 3, CV_32FC1);
	for (int i = 0; i < 3; i++)
	{
		float* pMatrixRow = Matrix.ptr<float>(i);
		for (int j = 0; j < 3; j++)
		{
			if (j == i)
				pMatrixRow[j] = 1;
			else
				pMatrixRow[j] = 0;
		}
	}

	//Tính toán theo công thức trên với ma trận này.
	for (int i = 0; i < 2; i++)
	{
		float* pMatrixRow = Matrix.ptr<float>(i);
		if (i == 0)
			pMatrixRow[2] = dx;
		else
			pMatrixRow[2] = dy;
	}

	//Nhân ma trận tạm với ma trận gốc _matrixTransform
	_matrixTransform = _matrixTransform * Matrix;
}

void AffineTransform::Rotate(float angle)
{
	//Tạo ma trận quay theo công thức:
	// (cos(angle),-sin(angle), 0)
	// (sin(angle), cos(angle), 0)
	// (0         , 0         , 1)

	//Đổi đơn vị biến angle sang radian
	angle = angle * 3.14 / 180;
	//Tạo 1 ma trận tạm để tính toán
	Mat Matrix = Mat(3, 3, CV_32FC1);
	for (int i = 0; i < 3; i++)
	{
		float* pMatrixRow = Matrix.ptr<float>(i);
		for (int j = 0; j < 3; j++)
		{
			if (j == i)
				pMatrixRow[j] = 1;
			else
				pMatrixRow[j] = 0;
		}
	}

	//Tính toán theo công thức trên với ma trận này.
	for (int i = 0; i < 2; i++)
	{
		float* pMatrixRow = Matrix.ptr<float>(i);
		if (i == 0)
		{
			pMatrixRow[0] = cos(angle);
			pMatrixRow[1] = (-1) * sin(angle);
		}
		else
		{
			pMatrixRow[0] = sin(angle);
			pMatrixRow[1] = cos(angle);
		}

	}

	//Nhân ma trận tạm với ma trận gốc _matrixTransform
	_matrixTransform = _matrixTransform * Matrix;
}

void AffineTransform::Scale(float sx, float sy)
{
	//Xây dựng ma trận theo công thức:
	// (sx,0,0)
	// (0,sy,0)
	// (0,0,1)

	//Tạo 1 ma trận tạm để tính toán
	Mat Matrix = Mat(3, 3, CV_32FC1);
	for (int i = 0; i < 3; i++)
	{
		float* pMatrixRow = Matrix.ptr<float>(i);
		for (int j = 0; j < 3; j++)
		{
			if (j == i)
				pMatrixRow[j] = 1;
			else
				pMatrixRow[j] = 0;
		}
	}

	//Tính toán theo công thức trên với ma trận này.
	for (int i = 0; i < 2; i++)
	{
		float* pMatrixRow = Matrix.ptr<float>(i);
		if (i == 0)
			pMatrixRow[0] = sx;
		else
			pMatrixRow[1] = sy;
	}

	//Nhân ma trận tạm với ma trận gốc _matrixTransform
	_matrixTransform = _matrixTransform * Matrix;
}

void AffineTransform::TransformPoint(float& x, float& y)
{
	//Tạo biến Matrix là ma trận nguồn, QMatrix là ma trận kết quả
	float Matrix[3] = { x, y, 1.0 };
	float QMatrix[3] = { 0.0, 0.0, 0.0 };

	//Nhân ma trận nguồn với _matrixTransform
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			QMatrix[i] += Matrix[j] * _matrixTransform.at<float>(i, j);
		}
	}
		
	//Gán lại giá trị mới cho điểm ảnh
	x = QMatrix[0];
	y = QMatrix[1];
}

AffineTransform::AffineTransform()
{
	//Tạo ma trận đơn vị cấp 3
	_matrixTransform = Mat(3, 3, CV_32FC1);
	for (int i = 0; i < 3; i++)
	{
		float* pMatrixRow = _matrixTransform.ptr<float>(i);
		for (int j = 0; j < 3; j++)
		{
			if (j == i)
				pMatrixRow[j] = 1;
			else
				pMatrixRow[j] = 0;
		}
	}
}

AffineTransform::~AffineTransform(){}

/*
Lớp thực hiện phép biến đổi hình học trên ảnh
*/
int GeometricTransformer::Transform(const Mat& beforeImage, Mat& afterImage, AffineTransform* transformer, PixelInterpolate* interpolator)
{
	if (beforeImage.empty())
		return 0;

	//Biến nChannels lưu số kênh màu ảnh gốc
	int nChannels = beforeImage.channels();
	//Biến srcWidthStep lưu khoảng cách giữa hai pixel trên 2 dòng liên tiếp nhau trong cùng 1 cột ảnh gốc
	int srcWidthStep = beforeImage.step[0];
	//Con trỏ pSrc trỏ đến vị trí đầu tiên của dữ liệu ảnh gốc
	uchar* pSrc = (uchar*)beforeImage.data;

	//Biến dstWidthStep lưu khoảng cách giữa hai pixel trên 2 dòng liên tiếp nhau trong cùng 1 cột ảnh đích
	int dstWidthStep = afterImage.step[0];
	//Con trỏ pDst trỏ đến vị trí đầu tiên của dữ liệu ảnh đích
	uchar* pDst = (uchar*)afterImage.data;
	
	float x, y;
	for (int i = 0; i < afterImage.rows; i++, pDst += dstWidthStep)
	{
		uchar* pRow = pDst;
		for (int j = 0; j < afterImage.cols; j++, pRow += nChannels)
		{
			x = i; y = j;
			// Tính giá trị từng vị trí tọa độ pixel theo matrix transform đã có của ảnh đích
			transformer->TransformPoint(x, y);
			if (round(x) >= 0 and round(y) >= 0 
					and round(x) < beforeImage.rows and round(y) < beforeImage.cols)
			{
				//Nếu tọa độ ảnh đích so với ảnh gốc nằm trong phạm vi ảnh gốc thì tiếp tục tính toán
				pRow[0] = interpolator->Interpolate(x, y, pSrc, srcWidthStep, nChannels, 0);
				pRow[1] = interpolator->Interpolate(x, y, pSrc, srcWidthStep, nChannels, 1);
				pRow[2] = interpolator->Interpolate(x, y, pSrc, srcWidthStep, nChannels, 2);
			}

		}
	}
	return 1;
}

int GeometricTransformer::RotateKeepImage(const Mat& srcImage, Mat& dstImage, float angle, PixelInterpolate* interpolator)
{
	if (srcImage.empty())
		return 0;

	return 1;
}

int GeometricTransformer::RotateUnkeepImage(const Mat& srcImage, Mat& dstImage, float angle, PixelInterpolate* interpolator)
{
	if (srcImage.empty())
		return 0;

	return 1;
}

int GeometricTransformer::Scale(const Mat& srcImage, Mat& dstImage, float sx, float sy, PixelInterpolate* interpolator)
{
	AffineTransform* transformer = new AffineTransform();
	transformer->Scale(1.0f / sx, 1.0f / sy);

	//Khai báo ảnh đích
	dstImage = Mat(round(srcImage.rows * sx), round(srcImage.cols * sy), srcImage.type());
	if (Transform(srcImage, dstImage, transformer, interpolator))
		return 1;
	return 0;
}

int GeometricTransformer::Resize(const Mat& srcImage, Mat& dstImage, int newWidth, int newHeight, PixelInterpolate* interpolator)
{
	AffineTransform* transformer = new AffineTransform();
	transformer->Scale(srcImage.rows * 1.0f / newWidth, srcImage.cols *1.0f / newHeight);

	//Khai báo ảnh đích
	dstImage = Mat(newWidth, newHeight, srcImage.type());
	if (Transform(srcImage, dstImage, transformer, interpolator))
		return 1;
	return 0;
}

int GeometricTransformer::Flip(const Mat& srcImage, Mat& dstImage, bool direction, PixelInterpolate* interpolator)
{
	AffineTransform* transformer = new AffineTransform();
	if (direction)
	{
		//Dùng hàm scale để đổi dấu tọa độ các pixel dựa vào ảnh đối xứng dọc hay ngang
		transformer->Scale(1.0, -1.0);
		//Dùng hàm translate để tịnh tiến điểm đã đổi dấu 1 đoạn srcImage.cols
		transformer->Translate(0, -srcImage.cols);
	}
	else 
	{
		// Dùng hàm scale để đổi dấu tọa độ các pixel dựa vào ảnh đối xứng dọc hay ngang
		transformer->Scale(-1.0, 1.0);
		//Dùng hàm translate để tịnh tiến điểm đã đổi dấu 1 đoạn srcImage.rows
		transformer->Translate(-srcImage.rows, 0);
	}
	//Khai báo ảnh đích
	dstImage = Mat(srcImage.rows, srcImage.cols, srcImage.type());
	if (Transform(srcImage, dstImage, transformer, interpolator))
		return 1;
	return 0;
}

GeometricTransformer::GeometricTransformer(){}
GeometricTransformer::~GeometricTransformer(){}