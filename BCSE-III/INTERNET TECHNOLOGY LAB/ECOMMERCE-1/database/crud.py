# from sqlalchemy.orm import Session
# from models import product, order, user

# # CRUD operations for Product
# def get_product(db: Session, product_id: int):
#     return db.query(product.Product).filter(product.Product.id == product_id).first()

# def create_product(db: Session, name: str, description: str, price: float, stock: int):
#     db_product = product.Product(name=name, description=description, price=price, stock=stock)
#     db.add(db_product)
#     db.commit()
#     db.refresh(db_product)
#     return db_product

# # CRUD operations for User and Order would follow similar patterns...
