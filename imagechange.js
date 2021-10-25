
//functions to alter the color of an image pixel by pixel 
let img = loadImageFromURL('https://pixabay.com/photos/marguerite-daisy-flower-white-729510/');
img.show();

//removeBlueAndGreen(img: Image): Image
function removeBlueAndGreen(img) {
  let result = img.copy();
  for(let i = 0; i < img.width; ++i) {
    for (let j = 0; j < img.height; ++j) {
      let pixel = img.getPixel(i,j);
      result.setPixel(i,j,[pixel[0], 0, 0]);
    }
  }

  return result;
}

//makeGrayscale(img: Image): Image
function makeGrayscale(img) {
    let result = img.copy();
  for(let i = 0; i < img.width; ++i) {
    for (let j = 0; j < img.height; ++j) {
      let pixel = img.getPixel(i,j);
      let m = Math.round((pixel[0]+pixel[1]+pixel[2])/3);
      result.setPixel(i,j,[m,m,m]);
    }
  }

  return result;
}

//imageMap(img: Image, func: (p: Pixel) => Pixel): Image
function imageMap(img, func) {
  let result = img.copy();
  for(let i = 0; i < img.width; ++i) {
    for (let j = 0; j < img.height; ++j) {
      result.setPixel(i,j, func(result.getPixel(i,j)));
    }
  }

  robot.show();
  result.show();

  return result;
}

//helper function for mapToRed
function redPixel(img)
{
  let orig = [img[0],0.0,0.0];
  return orig;
}

//mapToRed(img: Image): Image
function mapToRed(img) {
  return imageMap(img, redPixel);
  
}

//helper function for mapToRed
function GrayscalePixel(img)
{
  let orig = (img[0]+img[1]+img[2])/3;
  let pixel = [orig, orig, orig];
  return pixel;
}


//mapToGrayscale(img: Image): Image
function mapToGrayscale(img)
{
  return imageMap(img, GrayscalePixel);
}


//tests if red value changes in result of mapToGrayscale
test ('red value changes in result of mapToGrayscale', function() {
  const red = lib220.createImage(10,10,[1,0,0]);
  const result = mapToGrayscale(red);  
  for(let i = 0; i < 10; ++i) {
    for (let j = 0; j < 10; ++j) {
      const pixel = result.getPixel(i,j);
      assert (pixel[0] === 1/3);
    }
  }
});

//tests if imageMap functions correctly and creates a copy of the image

//tests if makeGrayScale works on an already grayscale image

test ('will makeGrayScale change an already grayscale image', function() {
  const white = lib220.createImage(10,10, [1,1,1]);
  const result = makeGrayscale(white);
  for(let i = 0; i < 10; ++i) {
    for (let j = 0; j < 10; ++j) {
      const pixel = result.getPixel(i,j);
      assert (pixel[0] === 1);
      assert (pixel[1] === 1);
      assert (pixel[2] === 1);
    }
  }
});

//tests if removeBlueAndGreen works on a completely red image
test ('will removeBlueAndGreen change an already red image', function() {
  const red = lib220.createImage(10,10, [1,0,0]);
  const result = removeBlueAndGreen(red);
  for(let i = 0; i < 10; ++i) {
    for (let j = 0; j < 10; ++j) {
      const pixel = result.getPixel(i,j);
      assert (pixel[0] === 1);
      assert (pixel[1] === 0);
      assert (pixel[2] === 0);
    }
  }
});