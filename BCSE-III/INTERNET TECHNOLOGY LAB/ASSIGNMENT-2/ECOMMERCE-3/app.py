from flask import Flask, render_template, redirect, url_for, request, flash
from flask_sqlalchemy import SQLAlchemy
from flask_login import LoginManager, UserMixin, login_user, logout_user, login_required, current_user
from models import db, User, Product, Cart

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///database.db'
app.config['SECRET_KEY'] = 'secretkey'
db.init_app(app)

login_manager = LoginManager()
login_manager.login_view = "login"
login_manager.init_app(app)

from flask_bcrypt import Bcrypt

bcrypt = Bcrypt(app)


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        username = request.form["username"]
        password = request.form["password"]
        hashed_password = bcrypt.generate_password_hash(password).decode('utf-8')

        user = User(username=username, password=hashed_password)
        db.session.add(user)
        db.session.commit()
        flash("Registration successful. Please log in.")
        return redirect(url_for("login"))
    return render_template("register.html")


@app.route("/add_product", methods=["GET", "POST"])
def add_product():
    if request.method == "POST":
        name = request.form["name"]
        price = float(request.form["price"])
        description = request.form["description"]

        product = Product(name=name, price=price, description=description)
        db.session.add(product)
        db.session.commit()
        flash("Product added successfully.")
        return redirect(url_for("home"))

    return render_template("add_product.html")
@app.route("/cart")
@login_required
def cart():
    cart_items = db.session.query(Cart, Product).join(Product, Cart.product_id == Product.id).filter(Cart.user_id == current_user.id).all()
    total_price = sum(item.Product.price * item.Cart.quantity for item in cart_items)
    return render_template("cart.html", cart_items=cart_items, total_price=total_price)

@app.route("/remove_from_cart/<int:cart_id>")
@login_required
def remove_from_cart(cart_id):
    cart_item = Cart.query.get(cart_id)
    if cart_item:
        db.session.delete(cart_item)
        db.session.commit()
    return redirect(url_for("cart"))
@app.route("/checkout")
@login_required
def checkout():
    cart_items = db.session.query(Cart, Product).join(Product, Cart.product_id == Product.id).filter(Cart.user_id == current_user.id).all()
    total_price = sum(item.Product.price * item.Cart.quantity for item in cart_items)

    if not cart_items:
        flash("Your cart is empty!")
        return redirect(url_for("cart"))

    new_order = Order(user_id=current_user.id, total_price=total_price)
    db.session.add(new_order)
    db.session.commit()

    Cart.query.filter_by(user_id=current_user.id).delete()
    db.session.commit()

    flash("Order placed successfully!")
    return redirect(url_for("orders"))
@app.route("/orders")
@login_required
def orders():
    user_orders = Order.query.filter_by(user_id=current_user.id).all()
    return render_template("orders.html", orders=user_orders)


@login_manager.user_loader
def load_user(user_id):
    return User.query.get(int(user_id))

@app.route("/")
def home():
    products = Product.query.all()
    return render_template("home.html", products=products)

@app.route("/login", methods=["GET", "POST"])
def login():
    if request.method == "POST":
        username = request.form["username"]
        password = request.form["password"]
        user = User.query.filter_by(username=username).first()
        if user and user.password == password:
            login_user(user)
            return redirect(url_for("home"))
        flash("Invalid credentials")
    return render_template("login.html")

@app.route("/logout")
@login_required
def logout():
    logout_user()
    return redirect(url_for("home"))

@app.route("/cart")
@login_required
def cart():
    cart_items = Cart.query.filter_by(user_id=current_user.id).all()
    return render_template("cart.html", cart_items=cart_items)

@app.route("/add_to_cart/<int:product_id>")
@login_required
def add_to_cart(product_id):
    item = Cart(user_id=current_user.id, product_id=product_id, quantity=1)
    db.session.add(item)
    db.session.commit()
    return redirect(url_for("cart"))

if __name__ == "__main__":
    with app.app_context():
        db.create_all()  # Creates database tables
    app.run(debug=True)
