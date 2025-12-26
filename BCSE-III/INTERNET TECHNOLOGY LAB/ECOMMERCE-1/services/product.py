from sqlalchemy.orm import Session
from models import product
from schemas.product import ProductCreate

# Create a new product
def create_product(db: Session, product_create_req: ProductCreate):
    new_product = product.Product(
        name=product_create_req.name,
        description=product_create_req.description,
        price=product_create_req.price,
        stock=product_create_req.stock
    )
    db.add(new_product)
    db.commit()
    db.refresh(new_product)
    return new_product

# Get a product by ID
def get_product_by_id(db: Session, product_id: int):
    return db.query(product.Product).filter(product.Product.id == product_id).first()

# Get all products
def get_products(db: Session, skip: int = 0, limit: int = 100):
    return db.query(product.Product).offset(skip).limit(limit).all()
