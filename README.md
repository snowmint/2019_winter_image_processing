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

20191105_wind
> wind effect

<table>
  <tr>
    <td>source</td>
    <td>wind effect</td>
    <td>heavy wind</td>
  </tr>
  <tr>
    <td><img alt="source" src="/20191105_wind/cap3_07a.bmp" width="200" /></td>
    <td><img alt="wind effect" src="/20191105_wind/wind.bmp" width="200" /></td>
    <td><img alt="heavy wind" src="/20191105_wind/heavy_wind.bmp" width="200" /></td>
  </tr>
</table>

<table>
  <tr>
    <td>left-down to right-up</td>
    <td>left-up to right-down</td>
    <td>right-down to left-up</td>
    <td>right-up to left-dow</td>
  </tr>
  <tr>
    <td><img alt="left-down to right-up" src="/20191105_wind/left-down_to_right-up.bmp" width="200" /></td>
    <td><img alt="left-up to right-down" src="/20191105_wind/left-up_to_right-down.bmp" width="200" /></td>
    <td><img alt="right-down to left-up" src="/20191105_wind/right-down_to_left-up.bmp" width="200" /></td>
    <td><img alt="right-up to left-down" src="/20191105_wind/right-up_to_left-down.bmp" width="200" /></td>
  </tr>
</table>

20191121_bigger_smaller
> Resize the image. Enlarge or shrink image sizes.

<table>
  <tr>
    <td>Shrink</td>
    <td>Source</td>
    <td>Enlarge</td>
  </tr>
  <tr>
    <td><img alt="shrink wind" src="/20191121_bigger_smaller/smaller.bmp" width="160" ></td>
    <td><img alt="source" src="/20191121_bigger_smaller/cap3_07a.bmp" width="200" ></td>
    <td><img alt="enlarge" src="/20191121_bigger_smaller/bigger.bmp" width="240" ></td>
  </tr>
</table>

20191128_lagrange
> affine deformation(仿射變換) and shear transformation(剪切變形)
> Use specify width and height to resize the image.

<table>
  <tr>
    <td>source</td>
    <td>resize with specify width height</td>
    <td>affine deformation</td>
    <td>shear transformation</td>
  </tr>
  <tr>
    <td><img alt="source" src="/20191128_lagrange/Lenna.bmp" width="200" ></td>
    <td><img alt="resize with specify width height" src="/20191128_lagrange/resize_with_specify_width_height.bmp" width="200" ></td>
    <td><img alt="affine deformation" src="/20191128_lagrange/affine_deformation.bmp" width="200" ></td>
    <td><img alt="shear transformation" src="/20191128_lagrange/shear_transformation.bmp" width="200" ></td>
  </tr>
</table>

20191205_transform
> Free-form deformation and Cylindrical_projection.

<table>
  <tr>
    <td>Source1: Chrysanthemum</td>
    <td>Free-form deformation</td>
    <td>Cylindrical_projection</td>
  </tr>
  <tr>
    <td><img alt="Source1" src="/20191205_transform/Chrysanthemum.jpg" width="200" ></td>
    <td><img alt="Free-form deformation" src="/20191205_transform/1-Free-form_deformation.bmp" width="200" ></td>
    <td><img alt="Cylindrical_projection" src="/20191205_transform/1-Cylindrical_projection.bmp" width="200" ></td>
  </tr>
</table>

<table>
  <tr>
    <td>Source2: stripes</td>
    <td>Free-form deformation</td>
    <td>Cylindrical_projection</td>
  </tr>
  <tr>
    <td><img alt="Source2" src="/20191205_transform/cap4_06a.bmp" width="200" ></td>
    <td><img alt="Free-form deformation" src="/20191205_transform/2-Free-form_deformation.bmp" width="200" ></td>
    <td><img alt="Cylindrical_projection" src="/20191205_transform/2-Cylindrical_projection.bmp" width="200" ></td>
  </tr>
</table>

20191212_colorSpace
> Free-form deformation and Cylindrical_projection.

<table>
  <tr>
    <td>Source1: Parisyan</td>
    <td>HSV - Skin capture</td>
    <td>Use "V" Get contour</td>
  </tr>
  <tr>
    <td><img alt="Parisyan" src="/20191212_colorSpace/Parisyan.jpg" width="200" ></td>
    <td><img alt="Skin capture" src="/20191212_colorSpace/Parisyan_Skin_capture.jpg" width="200" ></td>
    <td><img alt="Get contour" src="/20191212_colorSpace/Parisyan_Use_V(Value)_get_contour.jpg" width="200" ></td>
  </tr>
</table>

<table>
  <tr>
    <td>Source2: Diversity</td>
    <td>HSV - Skin capture</td>
    <td>Use "V" Get contour</td>
  </tr>
  <tr>
    <td><img alt="Parisyan" src="/20191212_colorSpace/Diversity.jpg" width="200" ></td>
    <td><img alt="Skin capture" src="/20191212_colorSpace/Diversity_Skin_capture.jpg" width="200" ></td>
    <td><img alt="Get contour" src="/20191212_colorSpace/Diversity_Use_V(Value)_get_contour.jpg" width="200" ></td>
  </tr>
</table>

<table>
  <tr>
    <td>Source3: different</td>
    <td>HSV - Skin capture</td>
    <td>Use "V" Get contour</td>
  </tr>
  <tr>
    <td><img alt="Parisyan" src="/20191212_colorSpace/different_skin_color.jpg" width="200" ></td>
    <td><img alt="Skin capture" src="/20191212_colorSpace/Skin_capture.jpg" width="200" ></td>
    <td><img alt="Get contour" src="/20191212_colorSpace/Use_V(Value)_get_contour.jpg" width="200" ></td>
  </tr>
</table>

20191219_colorConvert
> Color Reduce, Dithering, and draw the RGB histogram curve.

<table>
  <tr>
    <td>Source</td>
    <td>Color Reduce lv=10</td>
  </tr>
  <tr>
    <td><img alt="source" src="/20191219_colorConvert/HW12Test.png" width="400" ></td>
    <td><img alt="Color Reduce lv10" src="/20191219_colorConvert/Color_Reduce_lv10.png" width="400" ></td>
  </tr>
  <tr>
    <td>Color Reduce lv=5</td>
    <td>Color Reduce lv=2</td>
  </tr>
  <tr>
    <td><img alt="Color Reduce lv5" src="/20191219_colorConvert/Color_Reduce_lv5.png" width="400" ></td>
    <td><img alt="Color Reduce lv2" src="/20191219_colorConvert/Color_Reduce_lv2.png" width="400" ></td>
  </tr>
</table>

<table>
  <tr>
    <td>Dithering: Bayer</td>
    <td>Dithering: Shell</td>
  </tr>
  <tr>
    <td><img alt="Dithering Bayer" src="/20191219_colorConvert/Dithering_Bayer.png" width="400" ></td>
    <td><img alt="Dithering Shell" src="/20191219_colorConvert/Dithering_Shell.png" width="400" ></td>
  </tr>
</table>

<table>
  <tr>
    <td>RGB histogram curve</td>
  </tr>
  <tr>
    <td><img alt="RGB histogram curve" src="/20191219_colorConvert/【RGB直方圖-自製】.jpg" width="800" ></td>
  </tr>
</table>

20191226_imageStitching
> Image Stitching.

<table>
  <tr>
    <td>Source1</td>
    <td>Source2</td>
    <td>Image Stitching GIF</td>
  </tr>
  <tr>
    <td><img alt="Source1" src="/20191226_imageStitching/cap6_01a.bmp" width="400" ></td>
    <td><img alt="Source2" src="/20191226_imageStitching/cap6_01b.bmp" width="400" ></td>
    <td><img alt="Source2" src="/20191226_imageStitching/imageStitching.gif" width="400" ></td>
  </tr>
</table>