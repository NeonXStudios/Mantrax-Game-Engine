const { app, BrowserWindow, Menu, ipcMain, dialog } = require('electron');
const fs = require('fs');
const https = require('https');
const path = require('path');
const unzipper = require('unzipper');
const { exec } = require('child_process');

let mainWindow;
const configPath = path.join(app.getPath('userData'), 'config.json');


function loadConfig() {
    try {
        if (fs.existsSync(configPath)) {
            const data = fs.readFileSync(configPath, 'utf-8');
            console.log("Config loaded");
            return JSON.parse(data);
        }
    } catch (error) {
        console.error('Error reading config file:', error);
    }
    return {};
}

ipcMain.handle('load-config', () => {
    return loadConfig();
});

function saveConfig(config) {
    fs.writeFileSync(configPath, JSON.stringify(config, null, 2));
}

function createWindow() {
    const config = loadConfig();

    mainWindow = new BrowserWindow({
        width: 1500,
        height: 800,
        resizable: true,
        maximizable: false,
        minimizable: false,
        roundedCorners: true,
        icon: path.join(__dirname, 'assets/imgs/Logo.png'),
        frame: false,
        transparent: true,
        backgroundColor: '#00FFFFFF',
        hasShadow: true,
        webPreferences: {
            nodeIntegration: true,
            contextIsolation: false
        }
    });

    mainWindow.loadFile('index.html');
    mainWindow.setBackgroundColor('#00000000');
    Menu.setApplicationMenu(null);

    mainWindow.webContents.on('did-finish-load', () => {
        mainWindow.webContents.send('load-config', config);
    });

    mainWindow.webContents.openDevTools();
}

app.on('closed', () => {
    mainWindow = null;
});

app.whenReady().then(() => {
    createWindow();

    app.on('activate', () => {
        if (BrowserWindow.getAllWindows().length === 0) createWindow();
    });
});

app.on('closed', () => {
    app.quit();
});

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') app.quit();
});

ipcMain.on('close-app', () => {
    app.quit();
});

ipcMain.on('minimize-app', () => {
    if (mainWindow) {
        mainWindow.minimize();
    }
});

ipcMain.on('open-engine', async (event, programPath, parameter) => {

    if (!programPath || typeof programPath !== 'string') {
        console.error('La ruta del programa no es válida.');
        return;
    }

    if (!fs.existsSync(programPath)) {
        console.error(`El archivo no existe: ${programPath}`);
        return;
    }

    const programDirectory = path.dirname(programPath);

    try {
        const command = `"${programPath}" "${parameter}"`;

        console.log(`Ejecutando: ${command} en ${programDirectory}`);

        const childProcess = exec(
            command,
            { cwd: programDirectory },
            (error, stdout, stderr) => {
                if (error) {
                    console.error(`Error al ejecutar: ${error.message}`);
                    const stackTrace = new Error().stack;
                    fs.appendFileSync(logFilePath, `[${new Date().toISOString()}] Error al ejecutar: ${error.message}\nStacktrace:\n${stackTrace}\n`);
                }
                if (stderr) {
                    console.error(`Salida de error: ${stderr}`);
                }
                if (stdout) {
                    console.log(`Salida estándar: ${stdout}`);
                }
            }
        );

        const logFilePath = path.join(programDirectory, 'engine-crash.log');

        // Captura de datos de stderr
        childProcess.stderr.on('data', (data) => {
            console.error(`Error: ${data}`);
            const stackTrace = new Error().stack;
            fs.appendFileSync(logFilePath, `[${new Date().toISOString()}] STDERR: ${data}\nStacktrace:\n${stackTrace}\n`);
        });

        // Captura de datos de stdout
        childProcess.stdout.on('data', (data) => {
            console.log(`Salida estándar: ${data}`);
            fs.appendFileSync(logFilePath, `[${new Date().toISOString()}] STDOUT: ${data}\n`);
        });

        // Manejo de cierre del proceso
        childProcess.on('close', (code) => {
            console.log(`Proceso finalizado con código: ${code}`);
            if (code !== 0) {
                const stackTrace = new Error().stack;
                fs.appendFileSync(logFilePath, `[${new Date().toISOString()}] Proceso finalizó con código ${code}\nStacktrace:\n${stackTrace}\n`);
            }
        });
    } catch (err) {
        console.error(`Error al abrir el engine: ${err.message}`);
        const stackTrace = err.stack;
        const logFilePath = path.join(programDirectory, 'engine-crash.log');
        fs.appendFileSync(logFilePath, `[${new Date().toISOString()}] Error al abrir el engine: ${err.message}\nStacktrace:\n${stackTrace}\n`);
    }
});

ipcMain.on('maximize-app', () => {
    if (mainWindow) {
        if (mainWindow.isMaximized()) {
            mainWindow.restore();
        } else {
            mainWindow.maximize();
        }
    }
});

function extractZip(zipFilePath, outputDir, progressCallback) {
    return new Promise((resolve, reject) => {
        let totalEntries = 0;
        let extractedEntries = 0;

        fs.createReadStream(zipFilePath)
            .pipe(unzipper.Parse())
            .on('entry', (entry) => {
                totalEntries++;
                entry.autodrain();
            })
            .on('close', () => {
                fs.createReadStream(zipFilePath)
                    .pipe(unzipper.Parse())
                    .on('entry', (entry) => {
                        const fileName = entry.path;
                        const fullPath = path.join(outputDir, fileName);

                        try {
                            if (entry.type === 'Directory') {
                                fs.mkdirSync(fullPath, { recursive: true });
                                extractedEntries++;
                                if (progressCallback) {
                                    progressCallback(Math.round((extractedEntries / totalEntries) * 100));
                                }
                                entry.autodrain();
                            } else {
                                const dirName = path.dirname(fullPath);
                                fs.mkdirSync(dirName, { recursive: true });
                                entry.pipe(fs.createWriteStream(fullPath))
                                    .on('finish', () => {
                                        extractedEntries++;
                                        if (progressCallback) {
                                            progressCallback(Math.round((extractedEntries / totalEntries) * 100));
                                        }
                                    });
                            }
                        } catch (err) {
                            reject(err);
                        }
                    })
                    .on('close', resolve)
                    .on('error', reject);
            })
            .on('error', reject);
    });
}

ipcMain.on('download-engine', (event, downloadUrl) => {
    const downloadsPath = app.getPath('downloads');
    const filePath = path.join(downloadsPath, 'engine.zip');
    const extractPath = path.join(downloadsPath, 'editor');

    const file = fs.createWriteStream(filePath);
    let receivedBytes = 0;
    let totalBytes = 0;
    let startTime = Date.now();

    // Descargar el archivo ZIP
    https.get(downloadUrl, (response) => {
        if (response.statusCode !== 200) {
            event.reply('download-error', `Error en la descarga: ${response.statusCode}`);
            return;
        }

        totalBytes = parseInt(response.headers['content-length'], 10);

        response.on('data', (chunk) => {
            receivedBytes += chunk.length;
            const elapsedTime = (Date.now() - startTime) / 1000;
            const speed = (receivedBytes / elapsedTime) / 1024;

            event.reply('download-progress', {
                progress: Math.round((receivedBytes / totalBytes) * 100),
                downloaded: receivedBytes,
                total: totalBytes,
                speed: speed.toFixed(2)
            });
        });

        response.pipe(file);

        file.on('close', async () => {
            try {
                event.reply('extracting-start');

                await extractZip(filePath, extractPath, (progress) => {
                    event.reply('extract-progress', progress);
                });

                event.reply('download-complete', extractPath);
            } catch (err) {
                event.reply('download-error', `Error al extraer: ${err.message}`);
            }
        });
    }).on('error', (err) => {
        fs.unlink(filePath, () => { });
        event.reply('download-error', `Error en la descarga: ${err.message}`);
    });
});


ipcMain.handle('select-folder', async () => {
    const result = await dialog.showOpenDialog({
        properties: ['openDirectory'],
        title: 'Select Project Folder'
    });

    if (!result.canceled && result.filePaths.length > 0) {
        return result.filePaths[0];
    }
    return null;
});

ipcMain.handle('select-engine', async () => {
    const result = await dialog.showOpenDialog({
        properties: ['openFile'],
        title: 'Select Engine',
        filters: [
            { name: 'Executable Files', extensions: ['exe'] },
        ],
    });

    if (!result.canceled && result.filePaths.length > 0) {
        return result.filePaths[0];
    }
    return null;
});

ipcMain.on('open-dir', async (event, ruta) => {
    console.log(`Try opening folder from: ${ruta}`);

    fs.access(ruta, fs.constants.F_OK, (err) => {
        if (err) {
            console.error(`La ruta no existe: ${ruta}`);
            event.reply('open-dir-error', `La ruta no existe: ${ruta}`);
        } else {
            console.log(`Ruta válida: ${ruta}`);

            const rutaNormalizada = process.platform === 'win32' ? path.resolve(ruta).replace(/\//g, '\\') : ruta;

            let comando;

            if (process.platform === 'win32') {
                comando = `explorer "${rutaNormalizada}"`;
            } else if (process.platform === 'darwin') {
                comando = `open "${rutaNormalizada}"`;
            } else {
                comando = `xdg-open "${rutaNormalizada}"`;
            }

            exec(comando, (error) => {
                if (error) {
                    console.error(`Error abriendo la carpeta: ${error.message}`);
                    event.reply('open-dir-error', `Error abriendo la carpeta: ${error.message}`);
                } else {
                    console.log(`Carpeta abierta exitosamente: ${rutaNormalizada}`);
                    event.reply('open-dir-success', `Carpeta abierta exitosamente: ${rutaNormalizada}`);
                }
            });
        }
    });
});

ipcMain.on('save-config', (event, config) => {
    saveConfig(config);
});


ipcMain.on('create-project', async (event, data) => {
    const { projectName, includeAssets, projectPath } = data;

    console.log(`Proyecto: ${projectName}, Incluir Assets: ${includeAssets}, Ruta: ${projectPath}`);

    const pathToCreate = path.join(projectPath, projectName);

    try {
        await fs.promises.mkdir(pathToCreate, { recursive: true });

        if (includeAssets) {
            event.reply('extracting-start');

            await extractZip(`data/DefaultAssets.zip`, pathToCreate, (progress) => {
                event.reply('extract-progress', progress);
            });

            event.reply('download-complete', pathToCreate);
        }

        event.sender.send('create-project-success', { message: 'Proyecto creado exitosamente' });
    } catch (err) {
        console.error('Error durante la creación del proyecto:', err);
        event.sender.send('create-project-error', { error: `Error: ${err.message}` });
    }
});


ipcMain.handle('load-folders', async (event, projectPath) => {
    try {
        const folders = fs.readdirSync(projectPath, { withFileTypes: true })
            .filter((dirent) => dirent.isDirectory())
            .map((dirent) => dirent.name);

        console.log('Folders found:', folders);
        return folders;
    } catch (error) {
        console.error('Error loading folders:', error);
        throw new Error('Could not load folders.');
    }
});




ipcMain.on('delete-project', async (event, projectPath) => {
    try {
        if (!fs.existsSync(projectPath)) {
            throw new Error('La carpeta no existe.');
        }

        const files = await getAllFiles(projectPath);
        const totalFiles = files.length;
        let deletedFiles = 0;

        for (const file of files) {
            await deleteFileOrFolder(file);
            deletedFiles++;
            const progress = Math.round((deletedFiles / totalFiles) * 100);
            event.reply('delete-progress', progress);
        }

        await fs.promises.rm(projectPath, { recursive: true, force: true });

        event.reply('delete-complete');
    } catch (err) {
        console.error('Error on try delete:', err);
        event.reply('delete-error', err.message);
    }
});

async function getAllFiles(dirPath) {
    let files = [];
    const items = await fs.promises.readdir(dirPath, { withFileTypes: true });

    for (const item of items) {
        const fullPath = path.join(dirPath, item.name);
        if (item.isDirectory()) {
            files = files.concat(await getAllFiles(fullPath));
        } else {
            files.push(fullPath);
        }
    }

    return files;
}

async function deleteFileOrFolder(filePath) {
    const stats = await fs.promises.lstat(filePath);

    if (stats.isDirectory()) {
        await fs.promises.rm(filePath, { recursive: true, force: true });
    } else {
        await fs.promises.unlink(filePath);
    }
}
