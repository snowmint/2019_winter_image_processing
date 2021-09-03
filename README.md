# 2019_winter_image_processing
The homework of Image Processing with C++ and OpenCV.

20190912_setup
> Install Opencv and testing if it's work. 

20190919_bmpGenerate
> Use C to draw the BMP image, such as Damier Check, square, and rectangle. 

![Damier Check](/20190919_bmpGenerate/out.bmp)
![Square](/20190919_bmpGenerate/out1.bmp)
![Rectangle](/20190919_bmpGenerate/out2.bmp)

<p float="left">
  <img src="/20190919_bmpGenerate/Lenna.bmp" width="100" />
  <img src="/20190919_bmpGenerate/src1.bmp" width="100" />
  <img src="/20190919_bmpGenerate/src2.bmp" width="100" />
</p>


20190926_colorGenerate
> Histogram stretching and gamma correction (gamma < 1 lit, gamma > 1 dim)

![histogram stretching and Gamma correction](/20190926_colorGenerate/colorGenerate_result.png)
![Gamma correction](/20190926_colorGenerate/gamma_correction.png)

20191003_maskFilter
> Smoothing(加權平均平滑化), Sharpen(銳利化), and use self-made filter to get different effect

![Mask filter effect](/20191003_maskFilter/mask_effect.png)

20191016_cannyAndSobel
> Edge detection 邊緣偵測
> Sobel(邊緣偵測) and Canny(複合型邊緣偵測)

<p float="left">
  <img alt="source" src="/20191016_cannyAndSobel/cap3_02a.bmp" width="200" />
  <img alt="canny" src="/20191016_cannyAndSobel/canny.bmp" width="200" />
  <img alt="sobel" src="/20191016_cannyAndSobel/sobel.bmp" width="200" />
</p>

20191031_blurAndRelief
> Blur and Smoothing 模糊和平滑化
> Sobel smoothing with remain the edge (Sobel 邊緣保留平滑化) 和 median filtering 中值化

<p float="left">
  <img alt="source" src="/20191031_blurAndRelief/cap3_02a.bmp" width="200" />
  <img alt="sobel keep edge smoothing" src="/20191031_blurAndRelief/sobel_keep_edge_smoothing.bmp" width="200" />
  <img alt="median filtering" src="/20191031_blurAndRelief/median_filtering.bmp" width="200" />
</p>

> Radial blur(放射狀模糊化) and Relief tansformation(浮雕處理)

<p float="left">
  <img alt="source" src="/20191031_blurAndRelief/cap3_07a.bmp" width="200" />
  <img alt="Radial blur" src="/20191031_blurAndRelief/Radial_blur.bmp" width="200" />
  <img alt="relief transformation" src="/20191031_blurAndRelief/relief_transformation.bmp" width="200" />
</p>

20191105
> wind effect

<p float="left">
  <img alt="source" src="/20191105_wind/cap3_07a.bmp" width="200" />
  <img alt="wind effect" src="/20191105_wind/wind.bmp" width="200" />
  <img alt="heavy wind" src="/20191105_wind/heavy_wind.bmp" width="200" />
</p>

<p float="left">
  <img alt="left-down to right-up" src="/20191105_wind/left-down_to_right-up.bmp" width="200" />
  <img alt="left-up to right-down" src="/20191105_wind/left-up_to_right-down.bmp" width="200" />
  <img alt="right-down to left-up" src="/20191105_wind/right-down_to_left-up.bmp" width="200" />
  <img alt="right-up to left-down" src="/20191105_wind/right-up_to_left-down.bmp" width="200" />
</p>