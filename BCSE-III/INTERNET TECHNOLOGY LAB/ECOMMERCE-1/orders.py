from django.db import models
from .product import Products
from .customer import Customer
import datetime

class Order(models.Model):
	product = models.ForeignKey(Products,
								on_delete=models.CASCADE)
	customer = models.ForeignKey(Customer,
								on_delete=models.CASCADE)
	quantity = models.IntegerField(default=1)
	price = models.IntegerField()
	address = models.CharField(max_length=50, default='', blank=True)
	phone = models.CharField(max_length=50, default='', blank=True)
	date = models.DateField(default=datetime.datetime.today)
	status = models.BooleanField(default=False)

	def placeOrder(self):
		self.save()

	@staticmethod
	def get_orders_by_customer(customer_id):
		return Order.objects.filter(customer=customer_id).order_by('-date')

from django.contrib import admin
from django.urls import path
from .views.home import Index, store
from .views.signup import Signup
from .views.login import Login, logout
from .views.cart import Cart
from .views.checkout import CheckOut
from .views.orders import OrderView
from .middlewares.auth import auth_middleware


urlpatterns = [
	path('', Index.as_view(), name='homepage'),
	path('store', store, name='store'),

	path('signup', Signup.as_view(), name='signup'),
	path('login', Login.as_view(), name='login'),
	path('logout', logout, name='logout'),
	path('cart', auth_middleware(Cart.as_view()), name='cart'),
	path('check-out', CheckOut.as_view(), name='checkout'),
	path('orders', auth_middleware(OrderView.as_view()), name='orders'),

]
