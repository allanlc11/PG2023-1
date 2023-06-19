import ttkbootstrap as ttk, Filters as fil, Sticker as sticker, cv2 as cv, numpy as np
from tkinter import filedialog
from PIL import Image, ImageTk


WIDTH = 1200
HEIGHT = 800
global file_path, original, convertida, photo, frame
holding_sticker = ""

def cv2pil(img):
    return Image.fromarray(cv.cvtColor(img.copy(), cv.COLOR_BGR2RGB))
    
def pil2cv(img):
    return cv.cvtColor(np.asarray(img.copy()), cv.COLOR_BGR2RGB)


def abrir_imagem():
    global file_path, original, convertida, photo
    file_path = filedialog.askopenfilename(title="Abrir imagem", filetypes=[("Image Files", "*.jpg;*.jpeg;*.png;*.gif;*.bmp")])
    if file_path:
        original = cv.imread(file_path)
        convertida = cv2pil(original.copy())
        photo = ImageTk.PhotoImage(convertida.copy())
        canvas.create_image(photo.width()/2, photo.height()/2, image=photo)
        
def open_camera():
    global frame
    # Capture the video frame by frame
    original = vid.read()
    convertida = cv2pil(original.copy())
    photo = ImageTk.PhotoImage(convertida.copy())
    canvas.create_image(photo.width()/2, photo.height()/2, image=photo)
    canvas.after(1/60, open_camera)
    
        
def aplica():
    global original, convertida, holding_sticker
    holding_sticker = ""
    original = pil2cv(convertida)
       
def salva():
    global original
    cv.imwrite('salvo.jpg',original)
    
def reseta():
    global original, convertida, photo
    convertida = cv2pil(original.copy())
    photo = ImageTk.PhotoImage(convertida.copy())
    canvas.create_image(photo.width()/2, photo.height()/2, image=photo)
    
       
def vermelho():
    global file_path, original, convertida, photo
    if file_path:
        convertida = cv2pil(fil.red(original.copy()))
        photo = ImageTk.PhotoImage(convertida.copy())
        canvas.create_image(photo.width()/2, photo.height()/2, image=photo)

def azul():
    global file_path, original, convertida, photo
    if file_path:
        convertida = cv2pil(fil.blue(original.copy()))
        photo = ImageTk.PhotoImage(convertida.copy())
        canvas.create_image(photo.width()/2, photo.height()/2, image=photo)
       
def verde():
    global file_path, original, convertida, photo
    if file_path:
        convertida = cv2pil(fil.green(original.copy()))
        photo = ImageTk.PhotoImage(convertida.copy())
        canvas.create_image(photo.width()/2, photo.height()/2, image=photo)

def gray_arit():
    global file_path, original, convertida, photo
    if file_path:
        convertida = cv2pil(fil.gray_aritmetico(original.copy()))
        photo = ImageTk.PhotoImage(convertida.copy())
        canvas.create_image(photo.width()/2, photo.height()/2, image=photo) 
       
def gray_pond():
    global file_path, original, convertida, photo
    if file_path:
        convertida = cv2pil(fil.gray_ponderado(original.copy()))
        photo = ImageTk.PhotoImage(convertida.copy())
        canvas.create_image(photo.width()/2, photo.height()/2, image=photo) 

def roxo():
    global file_path, original, convertida, photo
    if file_path:
        convertida = cv2pil(fil.coloriza(original.copy()))
        photo = ImageTk.PhotoImage(convertida.copy())
        canvas.create_image(photo.width()/2, photo.height()/2, image=photo) 

def inverte():
    global file_path, original, convertida, photo
    if file_path:
        convertida = cv2pil(fil.inverte(original.copy()))
        photo = ImageTk.PhotoImage(convertida.copy())
        canvas.create_image(photo.width()/2, photo.height()/2, image=photo)    
       
def binario():
    global file_path, original, convertida, photo
    if file_path:
        convertida = cv2pil(fil.binariza(original.copy()))
        photo = ImageTk.PhotoImage(convertida.copy())
        canvas.create_image(photo.width()/2, photo.height()/2, image=photo) 

def blur():
    global file_path, original, convertida, photo
    if file_path:
        convertida = cv2pil(fil.blur(original.copy()))
        photo = ImageTk.PhotoImage(convertida.copy())
        canvas.create_image(photo.width()/2, photo.height()/2, image=photo)

def canny():
    global file_path, original, convertida, photo
    if file_path:
        convertida = cv2pil(fil.canny(original.copy()))
        photo = ImageTk.PhotoImage(convertida.copy())
        canvas.create_image(photo.width()/2, photo.height()/2, image=photo)  
       
def sobel():
    global file_path, original, convertida, photo
    if file_path:
        convertida = cv2pil(fil.sobel(original.copy()))
        photo = ImageTk.PhotoImage(convertida.copy())
        canvas.create_image(photo.width()/2, photo.height()/2, image=photo)

def dilata():
    global file_path, original, convertida, photo
    if file_path:
        convertida = cv2pil(fil.dilata(original.copy()))
        photo = ImageTk.PhotoImage(convertida.copy())
        canvas.create_image(photo.width()/2, photo.height()/2, image=photo)  
       
def erode():
    global file_path, original, convertida, photo
    if file_path:
        convertida = cv2pil(fil.erode(original.copy()))
        photo = ImageTk.PhotoImage(convertida.copy())
        canvas.create_image(photo.width()/2, photo.height()/2, image=photo)

def nitidez():
    global file_path, original, convertida, photo
    if file_path:
        convertida = cv2pil(fil.nitidez(original.copy()))
        photo = ImageTk.PhotoImage(convertida.copy())
        canvas.create_image(photo.width()/2, photo.height()/2, image=photo)
        
        
def estrela():    
    global holding_sticker
    holding_sticker = "Stickers/star.png"
    
def lua():
    global holding_sticker
    holding_sticker = "Stickers/moon.png"

def raio():
    global holding_sticker
    holding_sticker = "Stickers/bolt.png"

def smile():
    global holding_sticker
    holding_sticker = "Stickers/smile.png"

def coracao():
    global holding_sticker
    holding_sticker = "Stickers/heart.png"

def stick(x,y):
    global file_path, original, convertida, photo, holding_sticker
    if holding_sticker:
        if file_path:
            adesivo = cv.imread(holding_sticker, cv.IMREAD_UNCHANGED)
            pos_x = int(x-adesivo.shape[0]/2)
            pos_y = int(y-adesivo.shape[1]/2)
            convertida = cv2pil(sticker.overlay(original.copy(), adesivo, pos_x, pos_y))
            photo = ImageTk.PhotoImage(convertida.copy())
            canvas.create_image(photo.width()/2, photo.height()/2, image=photo)
    

def on_click(event):
    stick(event.x, event.y)

root = ttk.Window()
root.title("Editor de Imagens")
root.geometry("1200x800")
root.resizable(0, 0)


# Quadro de baixo com os filtros
filters_frame = ttk.Frame(root)
filters_frame.pack(side="left", fill="y")

# Quadro onde a imagem é desenhada
canvas = ttk.Canvas(root, width=WIDTH, height=HEIGHT)
canvas.bind("<Button-1>", on_click)
canvas.pack(pady=1, expand = ttk.YES)


#button1 = ttk.Button(filters_frame, text="Câmera", bootstyle="dark", command=command=open_camera)
#button1.pack(pady=1)

button2 = ttk.Button(filters_frame, text="Abrir", bootstyle="dark", command=abrir_imagem)
button2.pack(pady=1)

button3 = ttk.Button(filters_frame, text="Vermelho", bootstyle="dark", command=vermelho)
button3.pack(pady=1)

button4 = ttk.Button(filters_frame, text="Azul", bootstyle="dark", command=azul)
button4.pack(pady=1)

button5 = ttk.Button(filters_frame, text="Verde", bootstyle="dark", command=verde)
button5.pack(pady=1)

button6 = ttk.Button(filters_frame, text="Gray A", bootstyle="dark", command=gray_arit)
button6.pack(pady=1)

button7 = ttk.Button(filters_frame, text="Gray P", bootstyle="dark", command=gray_pond)
button7.pack(pady=1)

button8 = ttk.Button(filters_frame, text="Roxo", bootstyle="dark", command=roxo)
button8.pack(pady=1)

button9 = ttk.Button(filters_frame, text="Inverte", bootstyle="dark", command=inverte)
button9.pack(pady=1)

button10 = ttk.Button(filters_frame, text="Binariza", bootstyle="dark", command=binario)
button10.pack(pady=1)

button11 = ttk.Button(filters_frame, text="Blur", bootstyle="dark", command=blur)
button11.pack(pady=1)

button12 = ttk.Button(filters_frame, text="Canny", bootstyle="dark", command=canny)
button12.pack(pady=1)

button13 = ttk.Button(filters_frame, text="Sobel", bootstyle="dark", command=sobel)
button13.pack(pady=1)

button14 = ttk.Button(filters_frame, text="Dilata", bootstyle="dark", command=dilata)
button14.pack(pady=1)

button15 = ttk.Button(filters_frame, text="Erode", bootstyle="dark", command=erode)
button15.pack(pady=1)

button16 = ttk.Button(filters_frame, text="Nitidez", bootstyle="dark", command=nitidez)
button16.pack(pady=1)

button17 = ttk.Button(filters_frame, text="Aplica", bootstyle="dark", command=aplica)
button17.pack(pady=1)

button18 = ttk.Button(filters_frame, text="Salva", bootstyle="dark", command=salva)
button18.pack(pady=1)

button19 = ttk.Button(filters_frame, text="Reseta", bootstyle="dark", command=reseta)
button19.pack(pady=1)


button20 = ttk.Button(filters_frame, text="Estrela", bootstyle="dark", command=estrela)
button20.pack(side="bottom", pady=1)

button21 = ttk.Button(filters_frame, text="Lua", bootstyle="dark", command=lua)
button21.pack(side="bottom", pady=1)

button22 = ttk.Button(filters_frame, text="Raio", bootstyle="dark", command=raio)
button22.pack(side="bottom", pady=1)

button23 = ttk.Button(filters_frame, text="Smile", bootstyle="dark", command=smile)
button23.pack(side="bottom", pady=1)

button24 = ttk.Button(filters_frame, text="Coracao", bootstyle="dark", command=coracao)
button24.pack(side="bottom", pady=1)


root.mainloop()
