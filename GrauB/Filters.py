import numpy as np, cv2 as cv
            
            
def red(img):
    for i in range(img.shape[0]): #percorre linhas
        for j in range(img.shape[1]): #percorre colunas
            img.itemset((i,j,0),0) # canal B
            img.itemset((i,j,1),0) # canal G
    return img
    
def blue(img):
    for i in range(img.shape[0]): #percorre linhas
        for j in range(img.shape[1]): #percorre colunas
            img.itemset((i,j,1),0) # canal G
            img.itemset((i,j,2),0) # canal R
    return img
        
def green(img):
    for i in range(img.shape[0]): #percorre linhas
        for j in range(img.shape[1]): #percorre colunas
            img.itemset((i,j,0),0) # canal B
            img.itemset((i,j,2),0) # canal R
    return img
    

def gray_aritmetico(img):
    for i in range(img.shape[0]): #percorre linhas
        for j in range(img.shape[1]): #percorre colunas
            media = img.item(i,j,0) * 0.333 + img.item(i,j,1) * 0.333 + img.item(i,j,2) * 0.3333
            img.itemset((i,j,0),media) # canal B
            img.itemset((i,j,1),media) # canal G
            img.itemset((i,j,2),media) # canal R
    return img
    
def gray_ponderado(img):
    for i in range(img.shape[0]): #percorre linhas
        for j in range(img.shape[1]): #percorre colunas
            mediaPond = img.item(i,j,0) * 0.07 + img.item(i,j,1) * 0.71 + img.item(i,j,2) * 0.21
            img.itemset((i,j,0),mediaPond) # canal B
            img.itemset((i,j,1),mediaPond) # canal G
            img.itemset((i,j,2),mediaPond) # canal R
    return img
    
def coloriza(img):
    corModificadora = [125,0,125]    
    for i in range(img.shape[0]): #percorre linhas
        for j in range(img.shape[1]): #percorre colunas
            B = img.item(i,j,0) | corModificadora[0]
            G = img.item(i,j,1) | corModificadora[1]
            R = img.item(i,j,2) | corModificadora[2]
            img.itemset((i,j,0),B) # canal B
            img.itemset((i,j,1),G) # canal G
            img.itemset((i,j,2),R) # canal R
    return img
    
def inverte(img):
    for i in range(img.shape[0]): #percorre linhas
        for j in range(img.shape[1]): #percorre colunas
            img.itemset((i,j,0),img.item(i,j,0)^255) # canal B
            img.itemset((i,j,1),img.item(i,j,1)^255) # canal G
            img.itemset((i,j,2),img.item(i,j,2)^255) # canal R
    return img
    
def binariza(img):
    k = 200
    img2 = cv.cvtColor(img,cv.COLOR_BGR2GRAY)
    img3 = img2.copy()
    for i in range(img.shape[0]): #percorre linhas
        for j in range(img.shape[1]): #percorre colunas
            if img2.item(i,j) < k:
                img3.itemset((i,j),0)
            else:
                img3.itemset((i,j),255)
    return img3


def recorta(start_row, end_row, start_col, end_col):
    return img[start_row:end_row, start_col:end_col]

def redimensiona(width, heigh):
    return cv.resize(image, (width, heigh), interpolation= cv2.INTER_LINEAR)

def blur(img):    
    return cv.blur(img,(15,15))
    
def canny(img):   
    imgGrayscale =  cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    imgGaussianBlurred15x15 = cv.GaussianBlur(imgGrayscale,(15,15),0)
    return cv.Canny(imgGrayscale,50,100)
    
def sobel(img):
    ddepth = cv.CV_16S
    imgGrayscale =  cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    imgGaussianBlurred15x15 = cv.GaussianBlur(imgGrayscale,(15,15),0)
    
    grad_x = cv.Sobel(imgGaussianBlurred15x15, ddepth, 1, 0, ksize=3, borderType=cv.BORDER_DEFAULT)
    grad_y = cv.Sobel(imgGaussianBlurred15x15, ddepth, 0, 1, ksize=3, borderType=cv.BORDER_DEFAULT)
    abs_grad_x = cv.convertScaleAbs(grad_x)
    abs_grad_y = cv.convertScaleAbs(grad_y)
    
    grad = cv.addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0)
    
    return grad    
    
def dilata(img):
    kernel = np.ones((5, 5))
    return cv.dilate(img, kernel, iterations=1)
    
def erode(img):
    kernel = np.ones((5, 5))
    return cv.erode(img, kernel, iterations=1)
    
def nitidez(img):    
    kernel = np.array([
        [0, -1, 0],
        [-1, 5, -1],
        [0, -1, 0]
    ])        
    return cv.filter2D(img, ddepth=-1, kernel=kernel)
