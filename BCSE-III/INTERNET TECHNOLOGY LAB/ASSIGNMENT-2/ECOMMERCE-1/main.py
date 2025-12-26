from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles
from fastapi.responses import FileResponse
from database.connection import engine
from models import product, order, user
from routes import product as product_route, order as order_route, user as user_route
from services.user import create_admin_user

# Create tables in the database
product.Base.metadata.create_all(bind=engine)
order.Base.metadata.create_all(bind=engine)
user.Base.metadata.create_all(bind=engine)

app = FastAPI()

# Include routes
app.include_router(product_route.router)
app.include_router(order_route.router)
app.include_router(user_route.router)

@app.get("/")
async def get_index():
    return FileResponse("static/index.html")

# Mount static files on a subpath
app.mount("/static", StaticFiles(directory="static"), name="static")

create_admin_user()