from pydantic import BaseModel
from typing import List

# Schema for creating an order
class OrderCreate(BaseModel):
    user_id: int  # The user placing the order
    product_ids: List[int]  # The products being ordered

    class Config:
        orm_mode = True

# Schema for returning order data (response)
class OrderResponse(BaseModel):
    id: int
    user_id: int
    product_ids: List[int]
    # status: str  # Statuses like "pending", "completed", etc.

    class Config:
        orm_mode = True