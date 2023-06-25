''' Работа с jinja2'''

import json
from jinja2 import Template

index_path = 'index.html'
template_path = 'template.html'
json_path = 'data_file.json'


def get_profile_info(json_path):
    profile_info = {}
    with open(json_path, "r", encoding='utf-8') as file:
        json_file = json.load(file)
        for data in json_file:
            if data == 'login':
                profile_info['login'] = json_file[data]
            if data == 'id':
                profile_info['id'] = json_file[data]
            if data == 'html_url':
                profile_info['url'] = json_file[data]
            if data == 'name':
                profile_info['name'] = json_file[data]
    return profile_info


def get_profile_picture(json_path):
    with open(json_path, "r", encoding='utf-8') as file:
        json_file = json.load(file)
        for data in json_file:
            if data == 'avatar_url':
                profile_picture = json_file['avatar_url']
    return profile_picture


def create_template(template_path):
    template_html = open(template_path, encoding='utf-8').read()
    return Template(template_html)


if __name__ == '__main__':
    template = create_template(template_path)

    rendered_page = template.render(
        profile_info=get_profile_info(json_path),
        profile_picture=get_profile_picture(json_path)
    )

    with open(index_path, "w", encoding='utf-8') as file:
        file.write(rendered_page)
