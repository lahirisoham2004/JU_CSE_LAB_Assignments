# from sqlalchemy import Column, Integer, String, Float, ForeignKey
# from sqlalchemy.orm import relationship
# from database.connection import Base

# # Product Model
# class Product(Base):
#     __tablename__ = "products"

#     id = Column(Integer, primary_key=True, index=True)
#     name = Column(String, index=True)
#     description = Column(String, index=True)
#     price = Column(Float)
#     stock = Column(Integer)

#     orders = relationship("Order", back_populates="product")

# # Order Model
# class Order(Base):
#     __tablename__ = "orders"

#     id = Column(Integer, primary_key=True, index=True)
#     user_id = Column(Integer, ForeignKey("users.id"))
#     product_id = Column(Integer, ForeignKey("products.id"))

#     user = relationship("User", back_populates="orders")
#     product = relationship("Product", back_populates="orders")

# # User Model
# class User(Base):
#     __tablename__ = "users"

#     id = Column(Integer, primary_key=True, index=True)
#     username = Column(String, unique=True, index=True)
#     password = Column(String)

#     orders = relationship("Order", back_populates="user")
