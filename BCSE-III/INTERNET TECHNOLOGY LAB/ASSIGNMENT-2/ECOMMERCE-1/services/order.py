from sqlalchemy.orm import Session
from models import order, product
from typing import List
from schemas.order import OrderResponse

# Create a new order
def create_order(db: Session, user_id: int, product_ids: List[int]):
     # Retrieve the products using the provided product IDs
    products = db.query(product.Product).filter(product.Product.id.in_(product_ids)).all()
    
    # Check if all product IDs are valid
    if len(products) != len(product_ids):
        raise ValueError("One or more product IDs are invalid.")
    
    new_order = order.Order(user_id=user_id, products=products)
    db.add(new_order)
    db.commit()
    db.refresh(new_order)
    
    order_response = OrderResponse(
        id=new_order.id,
        user_id=new_order.user_id,
        product_ids=[product.id for product in new_order.products]  # Extract product IDs
    )
    
    return order_response

# Get orders by a specific user
def get_orders_by_user(db: Session, user_id: int):
    # Fetch orders by user_id and include their associated products
    orders = db.query(order.Order).filter(order.Order.user_id == user_id).all()

    # Prepare and return the response as a list of OrderResponse objects
    order_responses = [
        OrderResponse(
            id=order.id,
            user_id=order.user_id,
            product_ids=[product.id for product in order.products]  # Extract product IDs
        )
        for order in orders
    ]

    return order_responses
