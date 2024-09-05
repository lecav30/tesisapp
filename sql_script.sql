{% for table in tables %}
CREATE TABLE {{ table.name }} (
    id INT AUTO_INCREMENT PRIMARY KEY,
    {% for column in table.columns %}
    {{ column.name }} {{ column.type }}{% if not loop.is_last %},{% endif %}
    {% endfor %}
);
{% endfor %}