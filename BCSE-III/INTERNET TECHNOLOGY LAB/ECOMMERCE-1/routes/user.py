from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.orm import Session
from database.connection import SessionLocal
from services.user import create_user, get_user_from_db
from schemas.user import UserCreateOrReg, UserResponse
from utils.security import verify_password

router = APIRouter()

# Dependency
def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

@router.post("/users/", response_model=UserResponse)
def create_new_user(user: UserCreateOrReg, db: Session = Depends(get_db)):
    return create_user(db, user.username, user.password)

@router.get("/users/{user_id}", response_model=UserResponse)
def get_user(user_id: int, db: Session = Depends(get_db)):
    db_user = get_user_from_db(db, user_id=user_id)
    if db_user is None:
        raise HTTPException(status_code=404, detail="User not found")
    return db_user

@router.post("/login/", response_model=UserResponse)
def login_user(user: UserCreateOrReg, db: Session = Depends(get_db)):
    db_user = get_user_from_db(db, username=user.username)
    
    if not db_user or not verify_password(user.password, db_user.password):
        raise HTTPException(status_code=401, detail="Invalid username or password")
    
    return db_user  # TODO: return a JWT token instead
