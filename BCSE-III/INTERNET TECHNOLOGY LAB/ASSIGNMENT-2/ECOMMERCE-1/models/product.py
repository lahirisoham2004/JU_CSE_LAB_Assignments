from sqlalchemy import Column, Integer, String, Float
from sqlalchemy.orm import relationship
from database.connection import Base
from models.associations import order_product_association

class Product(Base):
    __tablename__ = "products"

    id = Column(Integer, primary_key=True, index=True)
    name = Column(String, index=True)
    description = Column(String)
    price = Column(Float)
    stock = Column(Integer)

    # Many-to-many relationship with Order via the junction table
    orders = relationship(
        "Order", secondary=order_product_association, back_populates="products"
    )