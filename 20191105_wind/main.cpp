#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace cv;

void effect_3_13(Mat img, Mat outimg, int ef, int ang);
void effect_3_14(Mat img, Mat outimg, double ef);
void effect_3_19(Mat img, Mat outimg);
void effect_3_23(Mat img, Mat outimg, Mat buffer, int act, int st, int sm);
int absi(int x);
int crgb(int x);
void adjustLightSourse();
int lighting(int g, double dx, double dy, double dz, double x, double y, double z);
int linearInterpolate(int *mat, int n, double x1, double y1);
int makeBlurPat(int n, int ang, int **x, int **y, int **val, int *sum);

double lx = -1.0, ly = -1.0, lz = 1.0;
double mil = 130;
double env = 30;
#define NN 17
#define NF 8

int main() {

	Mat src, res1, res2, res3, res4, res5, res6;
	src = imread("cap3_07a.bmp"); //test image
	imshow("src", src);


	res1 = src.clone();
	res2 = src.clone();
	res3 = src.clone();
	res4 = src.clone();
	res5 = src.clone();
	res6 = src.clone();

	effect_3_23(src, res1, res2, 2, 10, 20);
	imshow("3-23", res1);

	effect_3_14(src, res1, 50);
	effect_3_19(src, res2);
	effect_3_13(src, res1, 10, 10);
	effect_3_13(src, res2, 50, 10); //heavy wind
	effect_3_13(src, res3, 10, 50); //left-up to right-down
	effect_3_13(src, res4, 10, -50); //left-down to right-up
	effect_3_13(src, res5, 10, 150); //right-up to left-down
	effect_3_13(src, res6, 10, -150); //right-down to left-up

	imshow("wind", res1);
	imshow("heavy wind", res2);
	imshow("left-up to right-down", res3);
	imshow("left-down to right-up", res4);
	imshow("right-up to left-down", res5);
	imshow("right-down to left-up", res6);

	imwrite("wind.bmp", res1);
	imwrite("heavy_wind.bmp", res2);
	imwrite("left-up_to_right-down.bmp", res3);
	imwrite("left-down_to_right-up.bmp", res4);
	imwrite("right-up_to_left-down.bmp", res5);
	imwrite("right-down_to_left-up.bmp", res6);

	waitKey(0);
	return 0;
}
int absi(int x)
{
	if (x < 0) return -x;
	return x;
}

int crgb(int x)
{
	if (x < 0) return 0;
	if (x > 255) return 255;
	return x;
}
int getVal(int ed, int val, int con)
{
	int a, b;
	int v2;

	if (ed>255) ed = 255;
	v2 = (255 - 128)*val / 255 + 128;
	a = 255 - v2;
	b = -a * ed / 255 + 255;
	b = (b*con + val * (100 - con)) / 100;
	return b;
}

int linarPol(int c1, int c2, int c3, int c4, double xr, double yr)
{
	double d, e, f;
	int res;

	d = (double)c1*(1.0 - xr) + (double)c3*xr;
	e = (double)c2*(1.0 - xr) + (double)c4*xr;
	f = d * (1.0 - yr) + e * yr;
	res = (f + 0.5);
	if (res>255) res = 255;
	else if (res<0) res = 0;
	return res;
}

int getPatVal(int *mat, int n, int x, int y)
{
	if (x<0 || y<0 || x >= n || y >= n) return 0;

	return mat[x + y * n];
}

int linearInterpolate(int *mat, int n, double x1, double y1)
{
	int lx, ly;
	int adr;
	int mx, my, dep;
	int px, py;
	int nx, ny;
	double xrate, yrate;
	int col1, col2, col3, col4, col_n;

	nx = (int)(x1);
	ny = (int)(y1);
	px = (int)x1;
	py = (int)y1;
	xrate = x1 - (double)px;
	yrate = y1 - (double)py;
	col1 = getPatVal(mat, n, px, py);
	col2 = getPatVal(mat, n, px, py + 1);
	col3 = getPatVal(mat, n, px + 1, py);
	col4 = getPatVal(mat, n, px + 1, py + 1);
	return linarPol(col1, col2, col3, col4, xrate, yrate);
}

int makeBlurPat(int n, int ang, int **x, int **y, int **val, int *sum)
{
	int *tmpx, *tmpy, *tmpv;
	int *mat;
	int msum, tsum;
	int xx, yy, i;
	int vv;
	int hf;
	double rad;
	double fx, fy, tx, ty;
	double sinT, cosT;

	hf = n / 2;

	tmpx = (int*)malloc(sizeof(int)*n*n);
	tmpy = (int*)malloc(sizeof(int)*n*n);
	tmpv = (int*)malloc(sizeof(int)*n*n);
	mat = (int*)malloc(sizeof(int)*n*n);
	for (yy = 0; yy<n; yy++) {
		for (xx = 0; xx<n; xx++) {
			mat[xx + yy * n] = 0;
		}
	}
	msum = 0;
	for (xx = 0; xx<n; xx++) {
		vv = xx + 1;
		if (vv>hf + 1) vv = 0;
		mat[xx + hf * n] = vv;
		msum += vv;
	}

	rad = (double)ang*(3.14159265) / 180.0;
	sinT = sin(rad);
	cosT = cos(rad);

	i = 0;
	tsum = 0;
	for (yy = 0; yy<n; yy++) {
		for (xx = 0; xx<n; xx++) {
			tx = (double)(xx - hf);
			ty = (double)(yy - hf);
			fx = cosT * tx + sinT * ty + (double)hf;
			fy = -sinT * tx + cosT * ty + (double)hf;
			vv = linearInterpolate(mat, n, fx, fy);
			if (vv) {
				tmpx[i] = xx - hf;
				tmpy[i] = yy - hf;
				tmpv[i] = vv;
				tsum += tmpv[i];
				i++;
			}
		}
	}
	free(mat);
	*x = tmpx;
	*y = tmpy;
	*val = tmpv;
	*sum = tsum;
	return i;
}
void effect_3_23(Mat img, Mat outimg, Mat buffer, int act, int st, int sm)
{
	int val;
	int x,y;
	int xx,yy;

	int f1,f2;

	int err,egg,ebb;
	int sr_x,sg_x,sb_x;
	int sr_y,sg_y,sb_y;
	int r1,g1,b1,dum1,dum2;
	int rr,gg,bb,ed;
	int ro,go,bo;
	int ff;
	int st2,con;
	int sum;
	int endn,endn3,endo;
	Vec3b col, ncol, mcol;
	//ImageData *buf;
	int smooth[9];
	int fil[9]={
		-1,-1,-1,
		-1, 8,-1,
		-1,-1,-1};
	int sobel1[9]={
		 1, 0,-1,
		 2, 0,-2,
		 1, 0,-1};
	int sobel2[9]={
		 1, 2, 1,
		 0, 0, 0,
		-1,-2,-1};
	double	factor[7] = { 1.25, 1.5, 1.75, 2.0, 2.5, 3.0, 4.0 };	
	int x1,y1,x2,y2;

	x1 = 0;
	y1 = 0;
	x2 = img.cols - 1;
	y2 = img.rows - 1;


	sum=0;
	for(ff=0;ff<9;ff++) {
		if(ff==4) {
			smooth[ff]=10;
			sum+=10;
		}
		else {
			smooth[ff]=sm;
			sum+=sm;
		}
	}

	f1 = (int)((double)256 * factor[act]);
	f2 = (int)((double)256 * (factor[act] - 1.0)/2.0);
	//buf=createImage(img->width,img->height,24);

	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			if (y >= 0 && y < img.rows) {
				if (x >= 0 && x < img.cols) {
					col.val[2] = img.ptr<Vec3b>(y)[x].val[2];
					col.val[1] = img.ptr<Vec3b>(y)[x].val[1];
					col.val[0] = img.ptr<Vec3b>(y)[x].val[0];
					rr = col.val[2];
					gg = col.val[1];
					bb = col.val[0];
					ro = (rr*f1 - gg * f2 - bb * f2) / 256;
					go = (-rr * f2 + gg * f1 - bb * f2) / 256;
					bo = (-rr * f2 - gg * f2 + bb * f1) / 256;
					col.val[2] = ro;
					col.val[1] = go;
					col.val[0] = bo;
					buffer.at<Vec3b>(y, x) = col;
				}
			}	
			
		}
	}

	st2=st/2;
	con=66;

	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			err=egg=ebb=0;
			sr_x=sg_x=sb_x=0;
			sr_y=sg_y=sb_y=0;
			ff=0;
			for(yy=-1;yy<=1;yy++) {
				for(xx=-1;xx<=1;xx++) {
					if (y+yy >= 0 && y+yy < img.rows) {
						if (x+xx >= 0 && x+xx < img.cols) {
							//val = getPixel(img,x+xx,y+yy,&col);
							col.val[2] = img.ptr<Vec3b>(y+yy)[x+xx].val[2];
							col.val[1] = img.ptr<Vec3b>(y+yy)[x+xx].val[1];
							col.val[0] = img.ptr<Vec3b>(y + yy)[x+xx].val[0];

							err -= col.val[2] * fil[ff];
							egg -= col.val[1] * fil[ff];
							ebb -= col.val[0] * fil[ff];

							sr_x += col.val[2] * sobel1[ff];
							sg_x += col.val[1] * sobel1[ff];
							sb_x += col.val[0] * sobel1[ff];
							sr_y += col.val[2] * sobel2[ff];
							sg_y += col.val[1] * sobel2[ff];
							sb_y += col.val[0] * sobel2[ff];
						}
					}
					ff++;
					
				}
			}
			if (y >= 0 && y < img.rows) {
				if (x >= 0 && x < img.cols) {
					//val = getPixel(buf, x, y, &col);
					ncol.val[2] = buffer.ptr<Vec3b>(y)[x].val[2];
					ncol.val[1] = buffer.ptr<Vec3b>(y)[x].val[1];
					ncol.val[0] = buffer.ptr<Vec3b>(y)[x].val[0];
					r1 = ncol.val[2];
					g1 = ncol.val[1];
					b1 = ncol.val[0];

					err = err * st / 100 + (int)sqrt(sr_x*sr_x + sr_y * sr_y)*st2 / 100;
					egg = egg * st / 100 + (int)sqrt(sg_x*sg_x + sg_y * sg_y)*st2 / 100;
					ebb = ebb * st / 100 + (int)sqrt(sb_x*sb_x + sb_y * sb_y)*st2 / 100;
					rr = getVal(err, (int)ncol.val[2], con);
					gg = getVal(egg, (int)ncol.val[1], con);
					bb = getVal(ebb, (int)ncol.val[0], con);

					col.val[2] = rr;
					col.val[1] = gg;
					col.val[0] = bb;
					//setPixel(buf, x, y, &col);
					buffer.at<Vec3b>(y, x) = col;
				}
			}
		}
	}

	for(y=y1;y<=y2;y++) {
		for(x=x1;x<=x2;x++) {
			err=egg=ebb=0;
			ff=0;
			for(yy=-1;yy<=1;yy++) {
				for(xx=-1;xx<=1;xx++) {
					//val = getPixel(buf,x+xx,y+yy,&col);
					if (y + yy >= 0 && y + yy < img.rows) {
						if (x + xx >= 0 && x + xx < img.cols) {
							mcol.val[2] = buffer.ptr<Vec3b>(y + yy)[x + xx].val[2];
							mcol.val[1] = buffer.ptr<Vec3b>(y + yy)[x + xx].val[1];
							mcol.val[0] = buffer.ptr<Vec3b>(y + yy)[x + xx].val[0];
							err += mcol.val[2] *smooth[ff];
							egg += mcol.val[1] *smooth[ff];
							ebb += mcol.val[0] *smooth[ff];
						}
					}
					ff++;
				}
			}
			/*col.r = err/sum;
			col.g = egg/sum;
			col.b = ebb/sum;
			setPixel(outimg,x,y,&col);	*/
			if (y >= 0 && y < img.rows) {
				if (x >= 0 && x < img.cols) {
					col.val[2] = err / sum;
					col.val[1] = egg / sum;
					col.val[0] = ebb / sum;
					outimg.at<Vec3b>(y, x) = col;
				}
			}
		}
	}
	//disposeImage(buf);
	//return TRUE;
}
void effect_3_13(Mat img, Mat outimg, int ef, int ang) //風 ef 強度, ang 角度
{
	int val;
	int i;
	int x, y;
	int *px, *py, *pat, nn;
	int xx, yy;
	int rr, gg, bb, oo;
	int endn;
	Vec3b col, pixb, ncol;
	int mx;
	int pat_sum;
	int x1, y1, x2, y2;

	x1 = 0;
	y1 = 0;
	x2 = img.cols - 1;
	y2 = img.rows - 1;

	mx = 2 * ef + 1;

	nn = makeBlurPat(mx, ang, &px, &py, &pat, &pat_sum);
	cout << "nn : " << nn << endl;
	for (y = y1; y <= y2; y++) {
		for (x = x1; x <= x2; x++) {
			rr = gg = bb = oo = 0;
			for (i = 0; i < nn; i++) {
				if (y + py[i] >= 0 && y + py[i] < img.rows) {
					if (x + px[i] >= 0 && x + px[i] < img.cols) {
						ncol.val[2] = img.ptr<Vec3b>(y + py[i])[x + px[i]].val[2];
						ncol.val[1] = img.ptr<Vec3b>(y + py[i])[x + px[i]].val[1];
						ncol.val[0] = img.ptr<Vec3b>(y + py[i])[x + px[i]].val[0];
						rr += ncol.val[2] * pat[i];
						gg += ncol.val[1] * pat[i];
						bb += ncol.val[0] * pat[i];
						oo += pat[i];
					}
				}
			}
			if (y >= 0 && y < img.rows) {
				if (x >= 0 && x< img.cols) {
					col.val[2] = rr / (oo);
					col.val[1] = gg / (oo);
					col.val[0] = bb / (oo);
					outimg.at<Vec3b>(y, x) = col;
				}
			}
		}
	}
	free(px);
	free(py);
	free(pat);
}

void effect_3_14(Mat img, Mat outimg, double ef)// 放射狀模糊化
{
	int val;
	double rate;
	int i;
	int x, y;
	int xx, yy;
	int rr, gg, bb, oo;
	int endn;
	Vec3b col, ncol;
	int mx;
	int pat_sum, pat;
	double rad, dis, disI;
	double ox, oy;
	double disMAX;
	double dx, dy;
	int x1, y1, x2, y2;

	x1 = 0;
	y1 = 0;
	x2 = img.cols - 1;
	y2 = img.rows - 1;

	ox = (double)(x2 - x1) / 2.0;
	oy = (double)(y2 - y1) / 2.0;
	dx = (double)x2 - ox;
	dy = (double)y2 - oy;
	disMAX = sqrt(dx*dx + dy * dy);

	for (y = 0; y <= y2; y++) {
		for (x = 0; x <= x2; x++) {
			rr = gg = bb = oo = 0;
			dx = (double)x - ox;
			dy = (double)y - oy;
			if (dx != 0.0) rad = atan(dy / dx);
			else rad = 3.14159265 / 2.0;
			dis = sqrt(dx*dx + dy * dy);
			rate = ef * dis / disMAX;
			rate /= ((double)NF);
			pat_sum = 0;

			for (i = 0; i<NN; i++) {
				if (i == NF) pat = 3;
				else pat = 1;
				disI = (double)(i - NF)*rate;
				xx = (int)(disI*cos(rad)) + x;
				yy = (int)(disI*sin(rad)) + y;
				if (yy >= 0 && yy < img.rows) {
					if (xx >= 0 && xx < img.cols) {
						ncol.val[2] = img.ptr<Vec3b>(yy)[xx].val[2];
						ncol.val[1] = img.ptr<Vec3b>(yy)[xx].val[1];
						ncol.val[0] = img.ptr<Vec3b>(yy)[xx].val[0];
						rr += ncol.val[2] * pat;
						gg += ncol.val[1] * pat;
						bb += ncol.val[0] * pat;
						oo += pat;
						pat_sum += pat;
					}
				}
			}
			if (y >= 0 && y < img.rows) {
				if (x >= 0 && x< img.cols) {
					col.val[2] = rr / (oo);
					col.val[1] = gg / (oo);
					col.val[0] = bb / (oo);
					outimg.at<Vec3b>(y, x) = col;
				}
			}
		}
	}
}
void effect_3_19(Mat img, Mat outimg) //浮雕處理
{
	int val;
	int sobel1[9] = {
		1, 0,-1,
		2, 0,-2,
		1, 0,-1 };
	int sobel2[9] = {
		1, 2, 1,
		0, 0, 0,
		-1,-2,-1 };
	int x, y;
	int xx, yy;
	double dx, dy, dz;
	int hh;
	int du, dd, dl, dr;
	int endn;
	int c1, c2;
	int rrx, ggx, bbx;
	int rry, ggy, bby;
	int rr, gg, bb;
	Vec3b col, ncol, ocol;
	int *sobel;
	int sadr;
	int rate, vh;
	int gray;
	int x1, y1, x2, y2;

	x1 = 0;
	y1 = 0;
	x2 = img.cols - 1;
	y2 = img.rows - 1;

	adjustLightSourse();
	for (y = y1; y <= y2; y++) {
		for (x = x1; x <= x2; x++) {
			rrx = ggx = bbx = 0;
			rry = ggy = bby = 0;
			sadr = 0;
			for (yy = 0; yy<3; yy++) {
				for (xx = 0; xx<3; xx++) {
					if (y + yy - 1 >= 0 && y + yy - 1 < img.rows) {
						if (x + xx - 1 >= 0 && x + xx - 1 < img.cols) {
							col.val[2] = img.ptr<Vec3b>(y + yy - 1)[x + xx - 1].val[2];
							col.val[1] = img.ptr<Vec3b>(y + yy - 1)[x + xx - 1].val[1];
							col.val[0] = img.ptr<Vec3b>(y + yy - 1)[x + xx - 1].val[0];
							rr = col.val[2];
							gg = col.val[1];
							bb = col.val[0];
							gray = (bb * 28 + 77 * rr + gg * 151) / 256;
							rrx += gray * sobel1[sadr];
							rry += gray * sobel2[sadr];
						}
					}
					sadr++;
				}
			}
			dx = (double)rrx;
			dy = (double)rry;
			dz = 16.0;
			if (y + yy - 1 >= 0 && y + yy - 1 < img.rows) {
				if (x + xx - 1 >= 0 && x + xx - 1 < img.cols) {
					ncol.val[2] = img.ptr<Vec3b>(y + yy - 1)[x + xx - 1].val[2];
					ncol.val[1] = img.ptr<Vec3b>(y + yy - 1)[x + xx - 1].val[1];
					ncol.val[0] = img.ptr<Vec3b>(y + yy - 1)[x + xx - 1].val[0];
					rr = lighting(ncol.val[2], dx, dy, dz, (double)(x - x1), (double)(y - y1), 0.0);
					gg = lighting(ncol.val[1], dx, dy, dz, (double)(x - x1), (double)(y - y1), 0.0);
					bb = lighting(ncol.val[0], dx, dy, dz, (double)(x - x1), (double)(y - y1), 0.0);
					ocol.val[2] = rr;
					ocol.val[1] = gg;
					ocol.val[0] = bb;
					if (y >= 0 && y < img.rows) {
						if (x >= 0 && x< img.cols) {
							outimg.at<Vec3b>(y, x) = ocol;
						}
					}
				}
			}
		}
	}
}
void adjustLightSourse() {
	//please look 3-16
	double abl;

	abl = sqrt(lx*lx + ly * ly + lz * lz);
	lx = lx / abl;
	ly = ly / abl;
	lz = lz / abl;
}
int lighting(int g, double dx, double dy, double dz, double x, double y, double z)
{
	//please look 3-16
	double cosT, cosT2;
	double rx, ry, rz;
	double abl;
	double env2;
	int res;

	abl = sqrt(dx*dx + dy * dy + dz * dz);
	dx = dx / abl;
	dy = dy / abl;
	dz = dz / abl;
	cosT = dx * lx + dy * ly + dz * lz;
	rz = 2.0*cosT*dz - lz;
	if (rz>0.0)
		cosT2 = rz * rz*rz*rz*rz*rz*rz*rz*rz*rz*rz*rz;// n=12;
	else
		cosT2 = 0;
	if (cosT<0.0) cosT = 0.0;
	env2 = env * 255.0 / 100.0;
	res = (int)((cosT*255.0 + env2)*(double)g / 255.0);
	if (res>g) res = g;
	res += (mil*cosT2);
	if (res>255) res = 255;
	if (res<0) res = 0;
	return res;
}
