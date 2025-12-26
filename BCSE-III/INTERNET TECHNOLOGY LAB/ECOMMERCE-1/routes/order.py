from typing import List
from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.orm import Session
from database.connection import SessionLocal
from services.order import create_order, get_orders_by_user
from schemas.order import OrderCreate, OrderResponse

router = APIRouter()

# Dependency
def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

@router.post("/orders/", response_model=OrderResponse)
def create_new_order(order: OrderCreate, db: Session = Depends(get_db)):
    return create_order(db, order.user_id, order.product_ids)

@router.get("/orders/{user_id}", response_model=List[OrderResponse])
def get_user_orders(user_id: int, db: Session = Depends(get_db)):
    orders = get_orders_by_user(db, user_id)
    if not orders:
        raise HTTPException(status_code=404, detail="Orders not found")
    return orders
