from pydantic import BaseModel
from typing import Optional

# Schema for creating a user
class UserCreateOrReg(BaseModel):
    username: str
    password: str  # Store hashed passwords

    class Config:
        orm_mode = True

# Schema for returning user data (response)
class UserResponse(BaseModel):
    id: int
    username: str

    class Config:
        orm_mode = True
