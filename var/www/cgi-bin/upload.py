#!/usr/bin/python3

import cgi, os

print("Content-Type: text/html;charset=utf-8\r\n")

form = cgi.FieldStorage()

if 'filename' not in form:
    print("<H1>No file submitted</H1>")
else:
    fileitem = form['filename']
    if fileitem.filename:
        upload_dir = os.path.join(os.getcwd(), 'tmp')
        dest = os.path.join(upload_dir, os.path.basename(fileitem.filename))
        open(dest, 'wb').write(fileitem.file.read())
        message = 'The file "' + os.path.basename(fileitem.filename) + '" was uploaded to ' + upload_dir
    else:
        message = 'Uploading Failed'
    print("<H1> " + message + " </H1>")
