% return application
function [app] = func_intelhex ( file_name )

    % parametros
    APP_ADDRESS_MIN = hex2dec('00402000');
    APP_ADDRESS_MAX = hex2dec('00440000');

    fid = fopen(file_name);
    data = fread(fid);
    
    index = find(data == 58); % Find all ':'
    M = length(index); % length of lines
    row_N = 1;
    for i=1:M

        % guardar vector (quitar 2 de CRC y 2 extras que no se de donde salen)
        if (i < M)  vector = data( index(i) : index(i+1)-1-4 );
        else        vector = data( index(i) : end-4 );
        end

        % separar datos
        row.lenght = vector(2:3)';
        row.address = vector(4:7)';
        row.type = char(vector(8:9)');
        row.data = char(vector(10:end)');

        % dependiendo del tipo de dato realizar las operaciones
        % correspondinetes
        switch (row.type)
            case '00' % Datos
                % Encuentro direccion completa
                pos = hex2dec(char([pos_temp row.address]));
                if ((pos>=APP_ADDRESS_MIN) & (pos<APP_ADDRESS_MAX) )
                    if exist('app_temp')
                        app_temp = [app_temp row.data];
                    else
                        app_temp = row.data;
                    end
                end                  

            case '01' % Fin de archivo
                break

            case '02' % Dirección Extendida de Segmento (NO SOPORTADO)
                fprintf(2,'ERROR 2\n');
                break

            case '03' % Dirección de Comienzo de Segmento (NO SOPORTADO)
                fprintf(2,'ERROR 3\n');
                break

            case '04' % Dirección Lineal Extendida
                % guardo direccion
                pos_temp = row.data;

            case '05' % Comienzo de Dirección Lineal (NO SOPORTADO)
                fprintf(5,'ERROR 1\n');
                break
        end

        % borrar variable
        clear vector

    end

    % generar vector con bytes
    N = length(app_temp)/2;
    app = zeros(1,N);
    for i=0:(N-1)
        app(i+1) = hex2dec([app_temp(2*i+1) app_temp(2*i+2)]);  
    end
    
end