CREATE TABLE tasks (
    id INT PRIMARY KEY NOT NULL,
    frontend_id INT NOT NULL,
    title_slug TEXT NOT NULL,
    title TEXT,
    difficulty TEXT,
    content TEXT,
    topic_tags TEXT,
    code_snippets TEXT,
    paid_only BOOLEAN NOT NULL,
    refresh_time INT
);
