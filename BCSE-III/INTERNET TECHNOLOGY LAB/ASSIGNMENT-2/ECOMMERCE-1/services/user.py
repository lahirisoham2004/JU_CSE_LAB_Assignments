from sqlalchemy.orm import Session
from models import user
from utils.security import hash_password
from database.connection import SessionLocal

def create_admin_user():
    db: Session = SessionLocal()
    try:
        admin_user = db.query(user.User).filter(user.User.username == "admin").first()
        if not admin_user:
            hashed_password = hash_password("admin123")  # Set a strong default password
            admin_user = user.User(username="admin", password=hashed_password)
            db.add(admin_user)
            db.commit()
            db.refresh(admin_user)
            print("Admin user created successfully.")
        else:
            print("Admin user already exists.")
    finally:
        db.close()

# Create a new user with hashed password
def create_user(db: Session, username: str, password: str):
    hashed_password = hash_password(password)
    new_user = user.User(username=username, password=hashed_password)
    db.add(new_user)
    db.commit()
    db.refresh(new_user)
    return new_user

# Get a user
def get_user_from_db(db: Session, user_id: int = None, username: str = None):
    query = db.query(user.User)
    
    if user_id:
        return query.filter(user.User.id == user_id).first()
    
    if username:
        return query.filter(user.User.username == username).first()
    
    return None  # If neither user_id nor username is provided
