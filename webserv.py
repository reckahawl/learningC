from flask import Flask, request, render_template, redirect, url_for
from flask_login import LoginManager, login_user, login_required, current_user, logout_user

app = Flask(__name__)
app.secret_key = 'mysecretkey'  # Change this to a secure value in production

login_manager = LoginManager()
login_manager.init_app(app)

# Configure SSL/TLS
context = ('path/to/certfile', 'path/to/keyfile')

# Define a user class that implements Flask-Login's UserMixin
class User(UserMixin):
    def __init__(self, id):
        self.id = id

# Define a user loader function for Flask-Login
@login_manager.user_loader
def load_user(user_id):
    return User(user_id)

# Define routes for the login page and admin dashboard
@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        if username == 'admin' and password == 'password':
            user = User(username)
            login_user(user)
            return redirect(url_for('admin'))
        else:
            return render_template('login.html', error='Invalid username or password')
    else:
        return render_template('login.html')

@app.route('/admin')
@login_required
def admin():
    return render_template('admin.html')

# Define routes for the HTTP and HTTPS versions of the app
@app.route('/', methods=['GET'])
def index():
    return 'Hello, World! This is the HTTP version of the app.'

@app.route('/', methods=['GET'])
def index_https():
    return 'Hello, World! This is the HTTPS version of the app.'

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80, ssl_context=context, threaded=True)  # Listen on port 80 for HTTP requests
    app.run(host='0.0.0.0', port=443, ssl_context=context, threaded=True)  # Listen on port 443 for HTTPS requests
