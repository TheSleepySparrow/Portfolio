''' Модуль хранения конфигурации '''
import os
from dotenv import load_dotenv

load_dotenv()
GITHUB_LOGIN = os.getenv('GITHUB_LOGIN')
GITHUB_TOKEN = os.getenv('GITHUB_TOKEN')
