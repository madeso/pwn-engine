#define PWN_ENGINE_NO_AUTOLINK

#include "system_vfs.hpp"

#include "system.hpp"
#include <physfs.h>
#include <pwn/engine/game.h>
#include <pwn/assert.h>

#include <boost/filesystem.hpp>

#pragma comment (lib, "physfs.lib")

namespace pwn
{
	namespace engine
	{
		namespace // local
		{
			void ThrowPhysFsError()
			{
				const char* error = PHYSFS_getLastError();
				Assert(error);
				const std::string message = error;
				throw message;
			}

			bool& gPhysFsCreated()
			{
				static bool created = false;
				return created;
			}
		}

		struct PhysFsStarter
		{
			bool deinitInDestructor;

			PhysFsStarter(const pwn::string& argv0)
				: deinitInDestructor(false)
			{
				Assert(gPhysFsCreated()==false);
				const int result = PHYSFS_init(argv0.c_str());
				deinitInDestructor = true;
				if( result == 0) ThrowPhysFsError();
			}

			~PhysFsStarter()
			{
				if( deinitInDestructor )
				{
					PHYSFS_deinit();
				}
			}
		};

		namespace // local
		{
			const pwn::string GetParent(const pwn::string& app)
			{
				// system_complete solves relative paths we get from argv[0] and physfs seems give bad folders and data when it revieves them
				return boost::filesystem::system_complete(boost::filesystem::path(app).parent_path().parent_path() / "dummy.exe" ).file_string();
			}
		}

		class System_Vfs : public System
		{
		public:
			System_Vfs(const pwn::string& argv0, const pwn::string& company, const pwn::string& app)
			{
				{
					const pwn::string parentdir = GetParent(argv0);
					PhysFsStarter start(parentdir);
					const int result = PHYSFS_setSaneConfig(company.c_str(), app.c_str(), "vfs", 0, 0); // ignore cd-rom, append archives to search-list
					if( result == 0 ) ThrowPhysFsError();

					start.deinitInDestructor = false;
				}
				gPhysFsCreated() = true;
			}

			~System_Vfs()
			{
				PHYSFS_deinit();
				gPhysFsCreated() = false;
			}

			void update()
			{
				// nothing to do
			}
		};

		namespace
		{
			std::vector<pwn::string> FilesSeen(const pwn::string& dir)
			{
				std::vector<pwn::string> files;
				char **rc = PHYSFS_enumerateFiles(dir.c_str());
				char **i;

				for (i = rc; *i != NULL; i++)
				{
					files.push_back(*i);
				}

				PHYSFS_freeList(rc);

				return files;
			}

			void WriteFilesSeen()
			{
				std::vector<pwn::string> root = FilesSeen("");
			}
		}

		void SystemInstall_Vfs(Game* game, const pwn::string& argv0, const pwn::string& company, const pwn::string& app)
		{
			game->install(new System_Vfs(argv0, company, app));

			WriteFilesSeen();
		}

		/*
		FMOD_RESULT F_CALLBACK Open(const char *name, int unicode, unsigned int *filesize, void **handle, void **userdata) {
			if (name) {

				if ( !PHYSFS_exists(name) ) {
					return FMOD_ERR_FILE_NOTFOUND;
				}

				PHYSFS_file* fp = PHYSFS_openRead(name);
				if( !fp ) {
					return FMOD_ERR_FILE_BAD;
				}

				*filesize = PHYSFS_fileLength(fp);
				*userdata = (void *)0x12345678;
				*handle = fp;
			}

			return FMOD_OK;
		}

		FMOD_RESULT F_CALLBACK Close(void *handle, void *userdata) {
			if (!handle) {
				return FMOD_ERR_INVALID_PARAM;
			}
			PHYSFS_file* fp = (PHYSFS_file*) handle;
			PHYSFS_close(fp);

			return FMOD_OK;
		}

		FMOD_RESULT F_CALLBACK Read(void *handle, void *buffer, unsigned int sizebytes, unsigned int *bytesread, void *userdata) {
			if (!handle) {
				return FMOD_ERR_INVALID_PARAM;
			}

			if (bytesread) {
				PHYSFS_file* fp = (PHYSFS_file*) handle;
				*bytesread = (int)PHYSFS_read(fp, buffer, 1, sizebytes);
		    
				if (*bytesread < sizebytes) {
					return FMOD_ERR_FILE_EOF;
				}
			}

			return FMOD_OK;
		}

		FMOD_RESULT F_CALLBACK Seek(void *handle, unsigned int pos, void *userdata) {
			if (!handle) {
				return FMOD_ERR_INVALID_PARAM;
			}

			PHYSFS_file* fp = (PHYSFS_file*) handle;
			//fseek((FILE *)handle, pos, SEEK_SET);
			int result = PHYSFS_seek(fp, pos);
			if( result == 0 ) {
				return FMOD_ERR_FILE_COULDNOTSEEK;
			}
			return FMOD_OK;
		}

		typedef std::list<std::string> SoundSource;
		typedef std::map<std::string, std::string> SoundDefinitionMap2;
		typedef std::map<std::string, SoundSource> SoundDefinitionMap3;
		typedef std::map<std::string, FMOD::Sound*> StringSoundMap;
		typedef std::map<std::string, FMOD::Channel*> StringChannelMap;

		std::string AsValidPath(const std::string& iPath) {
			if( EndsWith(iPath, "/") ) {
				return iPath;
			}
			else {
				return iPath + "/";
			}
		}

		class SoundSystem : public System {
		public:
			SoundSystem() : System(GetNameAndVersion()), mSystem(0) {
				Assert(!gSoundSystem, "Already has a sound system");
				test( FMOD::System_Create(&mSystem), "creating system" );
				Assert(mSystem, "internal error, bug?");
				// A buffer of 0 means all reads go directly to the pointer specified. 2048 bytes is the size of a CD sector on most CD ISO formats so it is chosen as the default, for optimal reading speed from CD media.
				test(mSystem->setFileSystem(Open, Close, Read, Seek, 2048), "setting file system");
			*/
	}
}