import os
from werkzeug.utils import secure_filename

from flask import Flask, render_template, redirect, url_for, request
#from flask_bootstrap import Bootstrap
import numpy as np
import pandas as pd

app = Flask(__name__)

# VIDEO 1
#@app.route('/')
#def home():
#    return '<h1>Hello World!</h1>'

#@app.route('/user/<name>')
#def user(name):
#    return '<h1>Hello, {}!</h1>'.format(name)

#@app.route('/admin')
#def admin():
#    return redirect(url_for("user",name="Admin"))

# VIDEO 2: HTML TEMPLATES
# Using index.html
#@app.route('/')
#def home():
#    return render_template("index.html", content_1="Carlos", content_2=["A","B","C"])

#@app.route('/user/<name>')
#def user(name):
#    return render_template("index.html", content_1='Hello, {}!'.format(name), content_2=["carlos","perez","ricardo"])

# VIDEO 3: ADDING BOOTSTRAP
#@app.route("/")
#def home():
#    return render_template("index2.html", content="Testing")

# VIDEO 4: HTTP METHODS - GET / POST
@app.route("/")
def home():
    return render_template("index2.html")


@app.route("/login", methods=["POST","GET"])
def login():
    if request.method == "POST":
        user = request.form["nm"]
        csv_file = request.files["myfile"]

        print(type(csv_file))

        netflix = pd.read_csv(csv_file)
        print(netflix.head(20))
        return redirect(url_for("user", usr=user))
    else:
        return render_template("login.html")    

        # https://flask.palletsprojects.com/en/1.1.x/patterns/fileuploads/
        # https://blog.miguelgrinberg.com/post/handling-file-uploads-with-flask

        # check if the post request has the file part
        #if 'file' not in request.files:
        #    flash('No file part')
        #    return redirect(request.url)
        #file = request.files['file']
        # if user does not select file, browser also
        # submit an empty part without filename
        #if file.filename == '':
        #    flash('No selected file')
        #    return redirect(request.url)
        #if file and allowed_file(file.filename):
        #    filename = secure_filename(file.filename)
        #    file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
        #    return redirect(url_for('uploaded_file',
        #                            filename=filename))

#https://www.freecodecamp.org/news/how-to-build-a-web-application-using-flask-and-deploy-it-to-the-cloud-3551c985e492/

@app.route("/<usr>", methods=["POST","GET"])
def user(usr):
    return "<h1> {} </h1>".format(usr)    

if __name__ == '__main__':
    app.run(debug=True)


    
    