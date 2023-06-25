''' Основной модуль программы для запроса информации '''
import json
import requests
import config


USER_GIT = config.GITHUB_LOGIN
URL_GITHUB = 'https://api.github.com/users/' + USER_GIT
git = requests.get(URL_GITHUB, timeout=1000, headers={
                   'x-auth-token': config.GITHUB_TOKEN})

with open("data_file.json", "w", encoding='utf-8') as write_file:
    json.dump(git.json(), write_file)
