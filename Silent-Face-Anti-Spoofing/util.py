import os
import pickle

import tkinter as tk
from tkinter import messagebox
import face_recognition


def get_button(window, text, color, command, fg='white'):
    button = tk.Button(
                        window,
                        text=text,
                        activebackground="black",
                        activeforeground="white",
                        fg=fg,
                        bg=color,
                        command=command,
                        height=2,
                        width=20,
                        font=('Helvetica bold', 20)
                    )

    return button


def get_img_label(window):
    label = tk.Label(window)
    label.grid(row=0, column=0)
    return label


def get_text_label(window, text):
    label = tk.Label(window, text=text)
    label.config(font=("sans-serif", 21), justify="left")
    return label


def get_entry_text(window):
    inputtxt = tk.Text(window,
                       height=2,
                       width=15, font=("Arial", 32))
    return inputtxt


def msg_box(title, description):
    messagebox.showinfo(title, description)


def recognize(img, db_path = 'Silent-Face-Anti-Spoofing/db'):
    # it is assumed there will be at most 1 match in the db

    embeddings_unknown = face_recognition.face_encodings(img)
    if len(embeddings_unknown) == 0:
        return 'no_persons_found'
    else:
        embeddings_unknown = embeddings_unknown[0]

    db_dir = sorted(os.listdir(db_path))
    print(db_dir)
    match = False
    j = 0
    while not match and j < len(db_dir):
        path_ = os.path.join(db_path, db_dir[j])
        print(path_)

        file = open(path_, 'rb')
        embeddings = pickle.load(file)

        match = face_recognition.compare_faces([embeddings], embeddings_unknown)[0]
        j += 1

    if match:
        return db_dir[j - 1][:-7]
    else:
        return 'unknown_person'

# def recognize(img, db_path='Silent-Face-Anti-Spoofing/db'):
#     embeddings_unknown = face_recognition.face_encodings(img)
#     if len(embeddings_unknown) == 0:
#         return 'no_persons_found'
#     else:
#         embeddings_unknown = embeddings_unknown[0]

#     # Optimize using os.scandir for efficient iteration
#     for entry in os.scandir(db_path):
#         if entry.is_file():  # Check for files only
#             path_ = entry.path
#             file = open(path_, 'rb')
#             embeddings = pickle.load(file)

#             match = face_recognition.compare_faces([embeddings], embeddings_unknown)[0]
#             if match:
#                 return os.path.splitext(entry.name)[0]  # Return username without extension
#             file.close()  # Close file after each comparison

#     return 'unknown_person'