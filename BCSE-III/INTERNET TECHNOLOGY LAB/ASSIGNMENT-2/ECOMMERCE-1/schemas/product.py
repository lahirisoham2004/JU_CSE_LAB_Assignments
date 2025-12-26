from pydantic import BaseModel
from typing import List, Optional

# Schema for creating a product
class ProductCreate(BaseModel):
    name: str
    description: Optional[str] = None
    price: float
    stock: int

    class Config:
        orm_mode = True

# Schema for displaying a product
class ProductResponse(BaseModel):
    id: int
    name: str
    description: Optional[str] = None
    price: float
    stock: int

    class Config:
        orm_mode = True

# Schema for returning a list of products
class ProductListResponse(BaseModel):
    products: List[ProductResponse]

    class Config:
        orm_mode = True
